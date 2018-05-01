/**
 *MIT License
 * 
 * Copyright (c) 2017 Automa��o-IOT
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 */

#include "Dht.h"

#define MIN_INTERVAL 2000

#ifdef WIFI
    int Dht::counterDht = 0;

    Dht::Dht(GpiosEnum gpio,TypesDhtEnum tipo, unsigned long refreshDht, bool farenheit,String nameDHT)
    {
        resourceJson = "Dht"+String(counterDht++);
        subresourcesJson[indexJson][0] = "Temperatura";
        subresourcesJson[indexJson][1] = "Umidade";
        resourcesJson[indexJson++] = resourceJson;
        counterJson = counterDht;    
        refresh = refreshDht;
        this->gpio = gpio;	
        this->tipo = tipo;
        this->farenheit = farenheit;   
        name = nameDHT+String(counterDht);
        resourceWifiMode = true;
    }
#endif
    
Dht::Dht(long int idTemperature, long int idHumidity, GpiosEnum gpio, TypesDhtEnum tipo, unsigned long refreshDht,bool farenheit,String nameDHT)
{
    refresh = refreshDht;
    this->gpio = gpio;	
    this->tipo = tipo;
    this->farenheit = farenheit;    
    this->idTemperature=idTemperature;
    this->idHumidity=idHumidity;
    name = nameDHT+String(counterDht++);
    resourceWifiMode = false;
}

void  Dht::setModeConfigNetwork()
{
    #ifdef WIFI
        if(selectModeNetwork()==1) set(this->gpio,this->tipo,refresh,this->farenheit); else if(selectModeNetwork()==2) set(this->idTemperature,this->idHumidity,this->gpio,this->tipo,refresh,this->farenheit);
    #else
        set(this->idTemperature,this->idHumidity,this->gpio,this->tipo,refresh,this->farenheit);
    #endif  
    return;
}

void  Dht::set(long int idTemperature, long int idHumidity, GpiosEnum gpio, TypesDhtEnum tipo, unsigned long refresh,bool farenheit)
{
    this->statusTemperature=-1;
    this->statusHumidity=-1;        
    resourceWifi = true;
    begin(gpio, tipo,6);
    return;
}

#ifdef WIFI
    void  Dht::set(GpiosEnum gpio,TypesDhtEnum tipo, unsigned long refresh, bool farenheit)
    {
        idsResource(resourceJson);
        if((idr[0]!=0)&&(idr[1]!=0)) {
            this->idTemperature = idr[0];
            this->idHumidity = idr[1];
            set(this->idTemperature,this->idHumidity,gpio,tipo,refresh,farenheit);
        } else { 
            resourceWifi = false;
            debugIOT(MSG_ERROR,"Resource DHT referenced in main was not sent by JSON");
            refreshTimer.stop();
        }
        return;
    }
#endif

void  Dht::readTemperature()
{
    dht.temperature = readTemperatureSensor(this->farenheit);
    return;
}

void  Dht::readHumidity()
{
    dht.humidity = readHumiditySensor();
    return;
}

void  Dht::responseHttpCallback()
{
  if(!systemCall) {
    readTemperature();	
    readHumidity();
    refreshTimer.start();
  }
}

void  Dht::actionStart()
{
  
  if((clientDeviceHTTP!=200) ||(clientHTTP->code==403)) {
        serverConfig->url = deviceConfig.server+"/api/device/"+deviceConfig.publicKey+"/resources/"+String(int(this->idTemperature))+","+String(int(this->idHumidity));
        sendHTTP(BLANK,GET);
        return;
  }

  if(RequestQueueOn) {
    String heap = String(system_get_free_heap_size());
    debugIOT(MSG_INFO,name+" -  Memory HEAP",heap);  
    
    if(filter)
    {
      if((changeTemperature()) && (changeHumidity()))
      {
        doCallback(CallbackEvents::DATA_CHANGED);
        serverConfig->url = deviceConfig.server+"/api/device/"+deviceConfig.publicKey+"/resources/feeds";
        long int mount[30][10] = {{this->idTemperature,dht.temperature},{this->idHumidity,dht.humidity}};
        sendHTTP(json->parseJson(mount),POST);
      }
      else if(changeTemperature()) 
      {
        doCallback(CallbackEvents::DATA_CHANGED);
        serverConfig->url = deviceConfig.server+"/api/device/"+deviceConfig.publicKey+"/resource/"+String(this->idTemperature)+"/feeds/last";
        sendHTTP(json->parseJson(dht.temperature),POST); 
      }               
      else if (changeHumidity()) 
      {
        doCallback(CallbackEvents::DATA_CHANGED);
        serverConfig->url = deviceConfig.server+"/api/device/"+deviceConfig.publicKey+"/resource/"+String(this->idHumidity)+"/feeds/last";
        sendHTTP(json->parseJson(dht.humidity),POST);
      }
      else {
        serverConfig->url = deviceConfig.server+"/api/device/"+deviceConfig.publicKey+"/resources/"+String(int(this->idTemperature))+","+String(int(this->idHumidity));            
        sendHTTP(BLANK,GET);
      }
    } else {
      doCallback(CallbackEvents::DATA_CHANGED);
      serverConfig->url = deviceConfig.server+"/api/device/"+deviceConfig.publicKey+"/resources/feeds";        
      long int mount[30][10] = {{this->idTemperature,dht.temperature},{this->idHumidity,dht.humidity}};        
      sendHTTP(json->parseJson(mount),POST);
    }
  }
}

bool  Dht::changeTemperature()
{
    this->statusNew = dht.temperature;
    if(this->statusTemperature != this->statusNew)
    {
	this->statusTemperature = this->statusNew;
	return true;
    } else return false;
}

bool  Dht::changeHumidity()
{
    this->statusNew = dht.humidity;
    if(this->statusHumidity != this->statusNew)
    {
	this->statusHumidity = this->statusNew;
	return true;
    } else return false;
}

void  DhtSensor::begin(uint8_t pin, uint8_t type, uint8_t count) 
{
  this->pin = pin;
  this->type = type;
  #ifdef __AVR
    bit = digitalPinToBitMask(pin);
    port = digitalPinToPort(pin);
  #endif
  maxCycles = microsecondsToClockCycles(1000);  // 1 millisecond timeout for

  // set up the pins!
  pinMode(this->pin, INPUT_PULLUP);
  // Using this value makes sure that millis() - lastreadtime will be
  // >= MIN_INTERVAL right away. Note that this assignment wraps around,
  // but so will the subtraction.
  lastReadTime = -MIN_INTERVAL;
  DEBUG_PRINT("Max clock cycles: "); DEBUG_PRINTLN(maxCycles, DEC);
  return;
}

//boolean S == Scale.  True == Fahrenheit; False == Celcius
float  DhtSensor::readTemperatureSensor(bool S, bool force) {
  //float f = NAN;
    float f = 0;

  if (read(force)) {
    switch (this->type) {
    case _DHT11:
      f = data[2];
      if(S) {
        f = convertCtoF(f);
      }
      break;
    case _DHT22:
    case _DHT21:
      f = data[2] & 0x7F;
      f *= 256;
      f += data[3];
      f *= 0.1;
      if (data[2] & 0x80) {
        f *= -1;
      }
      if(S) {
        f = convertCtoF(f);
      }
      break;
    }
  }
  return f;
}

float  DhtSensor::convertCtoF(float c) {
  return c * 1.8 + 32;
}

float  DhtSensor::convertFtoC(float f) {
  return (f - 32) * 0.55555;
}

float  DhtSensor::readHumiditySensor(bool force) {
  //float f = NAN;
  float f = 0;
  if (read()) {
    switch (this->type) {
    case _DHT11:
      f = data[0];
      break;
    case _DHT22:
    case _DHT21:
      f = data[0];
      f *= 256;
      f += data[1];
      f *= 0.1;
      break;
    }
  }
  return f;
}

//boolean isFahrenheit: True == Fahrenheit; False == Celcius
float  DhtSensor::computeHeatIndex(float temperature, float percentHumidity, bool isFahrenheit) {
  // Using both Rothfusz and Steadman's equations
  // http://www.wpc.ncep.noaa.gov/html/heatindex_equation.shtml
  float hi;

  if (!isFahrenheit)
    temperature = convertCtoF(temperature);

  hi = 0.5 * (temperature + 61.0 + ((temperature - 68.0) * 1.2) + (percentHumidity * 0.094));

  if (hi > 79) {
    hi = -42.379 +
             2.04901523 * temperature +
            10.14333127 * percentHumidity +
            -0.22475541 * temperature*percentHumidity +
            -0.00683783 * pow(temperature, 2) +
            -0.05481717 * pow(percentHumidity, 2) +
             0.00122874 * pow(temperature, 2) * percentHumidity +
             0.00085282 * temperature*pow(percentHumidity, 2) +
            -0.00000199 * pow(temperature, 2) * pow(percentHumidity, 2);

    if((percentHumidity < 13) && (temperature >= 80.0) && (temperature <= 112.0))
      hi -= ((13.0 - percentHumidity) * 0.25) * sqrt((17.0 - abs(temperature - 95.0)) * 0.05882);

    else if((percentHumidity > 85.0) && (temperature >= 80.0) && (temperature <= 87.0))
      hi += ((percentHumidity - 85.0) * 0.1) * ((87.0 - temperature) * 0.2);
  }

  return isFahrenheit ? hi : convertFtoC(hi);
}

boolean  DhtSensor::read(bool force) {
  // Check if sensor was read less than two seconds ago and return early
  // to use last reading.
  uint32_t currenttime = millis();
  if (!force && ((currenttime - lastReadTime) < 2000)) {
    return lastResult; // return last correct measurement
  }
  lastReadTime = currenttime;

  // Reset 40 bits of received data to zero.
  data[0] = data[1] = data[2] = data[3] = data[4] = 0;

  // Send start signal.  See DHT datasheet for full signal diagram:
  //   http://www.adafruit.com/datasheets/Digital%20humidity%20and%20temperature%20sensor%20AM2302.pdf

  // Go into high impedence state to let pull-up raise data line level and
  // start the reading process.
  digitalWrite(this->pin, HIGH);
  delay(250);

  // First set data line low for 20 milliseconds.
  pinMode(this->pin, OUTPUT);
  digitalWrite(this->pin, LOW);
  delay(20);

  uint32_t cycles[80];
  {
    // Turn off interrupts temporarily because the next sections are timing critical
    // and we don't want any interruptions.
    InterruptLock lock;

    // End the start signal by setting data line high for 40 microseconds.
    digitalWrite(this->pin, HIGH);
    delayMicroseconds(40);

    // Now start reading the data line to get the value from the DHT sensor.
    pinMode(this->pin, INPUT_PULLUP);
    delayMicroseconds(10);  // Delay a bit to let sensor pull data line low.

    // First expect a low signal for ~80 microseconds followed by a high signal
    // for ~80 microseconds again.
    if (expectPulse(LOW) == 0) {
      DEBUG_PRINTLN(F("Timeout waiting for start signal low pulse."));
      lastResult = false;
      return lastResult;
    }
    if (expectPulse(HIGH) == 0) {
      DEBUG_PRINTLN(F("Timeout waiting for start signal high pulse."));
      lastResult = false;
      return lastResult;
    }

    // Now read the 40 bits sent by the sensor.  Each bit is sent as a 50
    // microsecond low pulse followed by a variable length high pulse.  If the
    // high pulse is ~28 microseconds then it's a 0 and if it's ~70 microseconds
    // then it's a 1.  We measure the cycle count of the initial 50us low pulse
    // and use that to compare to the cycle count of the high pulse to determine
    // if the bit is a 0 (high state cycle count < low state cycle count), or a
    // 1 (high state cycle count > low state cycle count). Note that for speed all
    // the pulses are read into a array and then examined in a later step.
    for (int i=0; i<80; i+=2) {
      cycles[i]   = expectPulse(LOW);
      cycles[i+1] = expectPulse(HIGH);
    }
  } // Timing critical code is now complete.

  // Inspect pulses and determine which ones are 0 (high state cycle count < low
  // state cycle count), or 1 (high state cycle count > low state cycle count).
  for (int i=0; i<40; ++i) {
    uint32_t lowCycles  = cycles[2*i];
    uint32_t highCycles = cycles[2*i+1];
    if ((lowCycles == 0) || (highCycles == 0)) {
      DEBUG_PRINTLN(F("Timeout waiting for pulse."));
      lastResult = false;
      return lastResult;
    }
    data[i/8] <<= 1;
    // Now compare the low and high cycle times to see if the bit is a 0 or 1.
    if (highCycles > lowCycles) {
      // High cycles are greater than 50us low cycle count, must be a 1.
      data[i/8] |= 1;
    }
    // Else high cycles are less than (or equal to, a weird case) the 50us low
    // cycle count so this must be a zero.  Nothing needs to be changed in the
    // stored data.
  }

  DEBUG_PRINTLN(F("Received:"));
  DEBUG_PRINT(data[0], HEX); DEBUG_PRINT(F(", "));
  DEBUG_PRINT(data[1], HEX); DEBUG_PRINT(F(", "));
  DEBUG_PRINT(data[2], HEX); DEBUG_PRINT(F(", "));
  DEBUG_PRINT(data[3], HEX); DEBUG_PRINT(F(", "));
  DEBUG_PRINT(data[4], HEX); DEBUG_PRINT(F(" =? "));
  DEBUG_PRINTLN((data[0] + data[1] + data[2] + data[3]) & 0xFF, HEX);

  // Check we read 40 bits and that the checksum matches.
  if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
    lastResult = true;
    return lastResult;
  }
  else {
    DEBUG_PRINTLN(F("Checksum failure!"));
    lastResult = false;
    return lastResult;
  }
}

// Expect the signal line to be at the specified level for a period of time and
// return a count of loop cycles spent at that level (this cycle count can be
// used to compare the relative time of two pulses).  If more than a millisecond
// ellapses without the level changing then the call fails with a 0 response.
// This is adapted from Arduino's pulseInLong function (which is only available
// in the very latest IDE versions):
//   https://github.com/arduino/Arduino/blob/master/hardware/arduino/avr/cores/arduino/wiring_pulse.c
uint32_t  DhtSensor::expectPulse(bool level) {
  uint32_t count = 0;
  // On AVR platforms use direct GPIO port access as it's much faster and better
  // for catching pulses that are 10's of microseconds in length:
  #ifdef __AVR
    uint8_t portState = level ? bit : 0;
    while ((*portInputRegister(port) & bit) == portState) {
      if (count++ >= maxCycles) {
        return 0; // Exceeded timeout, fail.
      }
    }
  // Otherwise fall back to using digitalRead (this seems to be necessary on ESP8266
  // right now, perhaps bugs in direct port access functions?).
  #else
    while (digitalRead(this->pin) == level) {
      if (count++ >= maxCycles) {
        return 0; // Exceeded timeout, fail.
      }
    }
  #endif

  return count;
}