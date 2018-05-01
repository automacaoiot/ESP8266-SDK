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

#include "Hcsr04.h"

#ifdef WIFI
    int Hcsr04::counterHcsr04 = 0;

    Hcsr04::Hcsr04(GpiosEnum trigger, GpiosEnum echo, unsigned long refreshHcsr04, uint16_t delay, uint8_t unit, String nameHSCR04)
    {
        resourceJson = "Hcsr04"+String(counterHcsr04++);
        resourcesJson[indexJson++] = resourceJson;
        counterJson = counterHcsr04;    
        resourceWifiMode = true;
        refresh = refreshHcsr04;
        this->trigger = trigger;
        this->echo = echo;
        this->delay = delay;
        this->unit = unit;   
        name = nameHSCR04+String(counterHcsr04);
    }
#endif

Hcsr04::Hcsr04(unsigned long id,GpiosEnum trigger, GpiosEnum echo, unsigned long refreshHcsr04, uint16_t delay, uint8_t unit, String nameHSCR04)
{
    resourceWifiMode = false;
    this->id = id;
    refresh = refreshHcsr04;
    this->trigger = trigger;
    this->echo = echo;
    this->delay = delay;
    this->unit = unit;
    name = nameHSCR04+id;
}

void  Hcsr04::setModeConfigNetwork()
{
    #ifdef WIFI
        if(selectModeNetwork()==1) set(this->trigger,this->echo,refresh,this->delay,this->unit); else if(selectModeNetwork()==2) set(this->id,this->trigger,this->echo,refresh,this->delay,this->unit);      
    #else
        set(this->id,this->trigger,this->echo,refresh,this->delay,this->unit);
    #endif  
    return;
}
    
void  Hcsr04::set(unsigned long id,GpiosEnum trigger, GpiosEnum echo, unsigned long refresh, uint16_t delay, uint8_t unit)
{
    resourceWifi = true;
    this->distanceHcsr04 = 0;
    pinMode(trigger, OUTPUT);
    pinMode(echo, INPUT);
    return;
}

#ifdef WIFI
    void  Hcsr04::set(GpiosEnum trigger, GpiosEnum echo, unsigned long refresh, uint16_t delay, uint8_t unit)
    {
        this->id = idResource(resourceJson);
        if(this->id!=0) set(this->id,trigger, echo,refresh,delay,unit); else 
        {
            resourceWifi = false;
            debugIOT(MSG_ERROR,"Resource Hcsr04 referenced in main was not sent by JSON");
            refreshTimer.stop();
        }
        return;	
    }
#endif

void  Hcsr04::responseHttpCallback()
{
    if(!systemCall) {
        read();
        if(this->unit = CM) this->distanceHcsr04 = distance.distanceCM; else this->distanceHcsr04 = distance.distanceINCHES;
        refreshTimer.start();
    }
    return;
}

void  Hcsr04::actionStart()
{

    if((clientDeviceHTTP!=200) ||(clientHTTP->code==403)) {
        serverConfig->url = deviceConfig.server+"/api/device/"+deviceConfig.publicKey+"/resource/"+String(this->id)+"/feeds/last";
        sendHTTP(BLANK,GET);
    }
    
    if(RequestQueueOn) {
        String heap = String(system_get_free_heap_size());
        debugIOT(MSG_INFO,name+" -  Memory HEAP",heap);  

        if(filter) {
            if(this->distanceHcsr04 != json->response->feed.rawData.toInt()) {        
                doCallback(CallbackEvents::DATA_CHANGED);
                serverConfig->url = deviceConfig.server+"/api/device/"+deviceConfig.publicKey+"/resource/"+String(this->id)+"/feeds";
                sendHTTP(json->parseJson(this->distanceHcsr04),POST);
            } else {
                serverConfig->url = deviceConfig.server+"/api/device/"+deviceConfig.publicKey+"/resource/"+String(this->id)+"/feeds/last";
                sendHTTP(BLANK,GET);
            }
        } else {
            doCallback(CallbackEvents::DATA_CHANGED);
            serverConfig->url = deviceConfig.server+"/api/device/"+deviceConfig.publicKey+"/resource/"+String(this->id)+"/feeds";
            sendHTTP(json->parseJson(this->distanceHcsr04),POST);   
        }
    }
    return;
}

void  Hcsr04::read()
{
    delayMicroseconds(2000);
    digitalWrite(this->trigger, LOW);
    delayMicroseconds(2);
    digitalWrite(this->trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(this->trigger, LOW);
    pinMode(this->echo, INPUT);
    this->durationCM = pulseIn(this->echo, HIGH,this->delay);
    distance.distanceCM = microsecondsToCentimeters(this->durationCM);
    distance.distanceINCHES = distance.distanceCM * 0.39370079;
    return;
}

int  Hcsr04::microsecondsToCentimeters(int microseconds)
{
    // The speed of sound is 340 m/s or 29 microseconds per centimeter.
    // The ping travels out and back, so to find the distance of the
    // object we take half of the distance travelled.
    return (microseconds / 29 / 2);
}