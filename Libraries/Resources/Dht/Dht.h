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

#ifndef INCLUDE_DHT_H_
#define INCLUDE_DHT_H_

#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include "BaseDevice.h"
#include "WProgram.h"

/**
 * Define types of sensors DHT
 */
#define _DHT11 11
#define _DHT22 22
#define _DHT21 21
#define DEBUG_PRINTER Serial

#ifdef DHTSENSOR_DEBUG
  #define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
  #define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
  #define DEBUG_PRINT(...) {}
  #define DEBUG_PRINTLN(...) {}
#endif

/**
 * Class DHTSENSOR Set up and executing the resource
 */
class DhtSensor {
  public:
    DhtSensor() {};
   /**
    * Set DHT
    * @param pin
    * @param type
    * @param count
    */ 
   void begin(uint8_t pin, uint8_t type, uint8_t count=6);
   /**
    * Read temperature
    * @param temperature
    * @param percentHumidity
    * @param isFahrenheit
    * @return Temperature
    */
   float readTemperatureSensor(bool S=false, bool force=false);
   /**
    * Convert temperatur Celsius to Farenheit
    * @param Temperature Celsius
    * @return Temperature Farenheit
    */
   float convertCtoF(float);
   /**
    * Convert temperature Farenheit to Celsius
    * @param Temperature Farenheit
    * @return Temperature Celsius
    */
   float convertFtoC(float);
   /**
    * Calculate temperature Using both Rothfusz and Steadman's equations
    * @param level
    * @return Temperature
    */
   float computeHeatIndex(float temperature, float percentHumidity, bool isFahrenheit=true);
   /**
    * Read Humidity sensor DHT
    * @param force
    * @return Humidity
    */
   float readHumiditySensor(bool force=false);
   /**
    * Check if sensor was read less than two seconds ago and return early
    * @param force
    * @return True ou False
    */
   boolean read(bool force=false);

 private:
  uint8_t data[5];
  uint8_t pin, type;
  #ifdef __AVR
    // Use direct GPIO access on an 8-bit AVR so keep track of the port and bitmask
    // for the digital pin connected to the DHT.  Other platforms will use digitalRead.
    uint8_t bit, port;
  #endif
  uint32_t lastReadTime, maxCycles;
  bool lastResult;

  /**
   * Expect the signal line to be at the specified level for a period of time and
   * @param level
   * @return a count of loop cycles spent at that level
   */
  uint32_t expectPulse(bool level);

};

class InterruptLock {
  public:
   InterruptLock() {
    noInterrupts();
   }
   ~InterruptLock() {
    interrupts();
   }

};


/**
 * Class DHT Set up and executing the resource 
 */
class Dht : public BaseDevice , public DhtSensor
{
	private:
           
            uint16_t statusTemperature;		
            uint16_t statusHumidity;		
            int _TypeDHT;		
            long int value[10]; 
            long int idTemperature; 
            long int idHumidity;
            TypesDhtEnum tipo;		
            bool farenheit;	            
            GpiosEnum gpio;
            uint16_t statusNew;

            #ifdef WIFI
                static int counterDht;
                /**
                * Set the resource over the WIFI network
                * @param gpio GPIO associated with the resource
                * @param refresh Resource timer update in milliseconds
                * @param tipo Define type DHT 11 21 22 33 44                
                * @param farenheit ReadTemperature Celsius False Farenheit True
                */
                void set(GpiosEnum gpio,TypesDhtEnum tipo, unsigned long refreshDht, bool farenheit=false);                            
            #endif                

            /**
             * Set the resource manually
             * @params value Array of ID resource and value temperature/humidity
             * @params gpio GPIO associated with the resource
             * @params tipo Define type DHT 11 21 22 33 44
             * @params refresh Resource timer update in milliseconds
             * @params farenheit ReadTemperature Celsius False Farenheit True
             */            
            void set(long int idTemperature, long int idHumidity, GpiosEnum gpio, TypesDhtEnum tipo, unsigned long refreshDht,bool farenheit=false);		            
            
            /**
             * Check if temperature change
             * @return True or False
             */
            bool changeTemperature();
            /**
             * Check if Humidity change
             * @return True or False
             */
            bool changeHumidity();
            /**
             * Performs action through the timer
             */
            virtual void actionStart();
            /**
             * Perform resource action after receiving callback
             */
            virtual void responseHttpCallback();
            /**
             * set mode Config Network 
             * WIFI or Normal
             */
            virtual void setModeConfigNetwork();
            /**
             * Read Temperatur DHT and write in the struct dht
             */
            void readTemperature();
            /**
             * Read Humidity DHT and write in the struct dht
             */            
            void readHumidity();
                              
	protected:		
				
	public:

            Dht(GpiosEnum gpio,TypesDhtEnum tipo, unsigned long refreshDht=1000, bool farenheit=false,String nameDHT="Dht");            
            Dht(long int idTemperature, long int idHumidity, GpiosEnum gpio, TypesDhtEnum tipo, unsigned long refreshDht=1000,bool farenheit=false,String nameDHT="Dht");
            virtual ~Dht(){};
            
            DatadhtEnum dht; /**< Structure dht */            
};

#endif