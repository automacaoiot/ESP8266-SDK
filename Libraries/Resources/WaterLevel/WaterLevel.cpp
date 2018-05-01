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

#include "WaterLevel.h" 

#ifdef WIFI
    int WaterLevel::counterWaterLevel = 0;

    WaterLevel::WaterLevel(GpiosEnum gpio, unsigned long refreshWaterLevel,String nameWATERLEVEL)
    {    
        resourceJson = "WaterLevel"+String(counterWaterLevel++);
        resourcesJson[indexJson++] = resourceJson;
        counterJson = counterWaterLevel;
        refresh = refreshWaterLevel;
        this->gpio = gpio;
        name = nameWATERLEVEL+String(counterWaterLevel);
        resourceWifiMode=true;    
    }
#endif

WaterLevel::WaterLevel(unsigned long id, GpiosEnum gpio, unsigned long refreshWaterLevel,String nameWATERLEVEL)
{    
    this->id = id;	
    refresh = refreshWaterLevel;
    this->gpio = gpio;        
    name = nameWATERLEVEL+id;
    resourceWifiMode=false;    
}

void  WaterLevel::responseHttpCallback()
{
    if(!systemCall) {
        this->levelFILTER=read();
        refreshTimer.start();    
    }
    return;
}

void  WaterLevel::actionStart()
{    
    if((clientDeviceHTTP!=200) ||(clientHTTP->code==403)) {
        serverConfig->url = deviceConfig.server+"/api/device/"+deviceConfig.publicKey+"/resource/"+String(this->id)+"/feeds/last";    
        sendHTTP(BLANK,GET);
    }
    
    if(RequestQueueOn) {
        String heap = String(system_get_free_heap_size());
        debugIOT(MSG_INFO,name+" -  Memory HEAP",heap);  
       
        if(filter) {
            if(this->levelFILTER != json->response->feed.rawData.toInt()) {
                doCallback(CallbackEvents::DATA_CHANGED);
                serverConfig->url = deviceConfig.server+"/api/device/"+deviceConfig.publicKey+"/resource/"+String(this->id)+"/feeds";    
                sendHTTP(json->parseJson(this->levelFILTER),POST);
            } else {
                serverConfig->url = deviceConfig.server+"/api/device/"+deviceConfig.publicKey+"/resource/"+String(this->id)+"/feeds/last";    
                sendHTTP(BLANK,GET);
            }
        }else {
            doCallback(CallbackEvents::DATA_CHANGED);
            serverConfig->url = deviceConfig.server+"/api/device/"+deviceConfig.publicKey+"/resource/"+String(this->id)+"/feeds";        
            sendHTTP(json->parseJson(this->levelFILTER),POST);
        }    
    }
    return;
}


void  WaterLevel::setModeConfigNetwork()
{    
    #ifdef WIFI
        if(selectModeNetwork()==1) set(this->gpio,refresh); else if(selectModeNetwork()==2) set(this->id,this->gpio,refresh); 
    #else
        set(this->id,this->gpio,refresh);
    #endif  
    return;
}

void   WaterLevel::set(unsigned long id, GpiosEnum gpio, unsigned long refresh)
{
    resourceWifi = true;
    pinMode(gpio, OUTPUT);
    digitalWrite(gpio, HIGH);
    return;
}

#ifdef WIFI
    void  WaterLevel::set(GpiosEnum gpio, unsigned long refresh)
    {
        this->id = idResource(resourceJson);
        this->gpio=gpio;
        if(this->id!=0) set(this->id,gpio,refresh); else 
        { 
            resourceWifi = false;                
            debugIOT(MSG_ERROR,"Resource WATER LEVEL referenced in main was not sent by JSON");
            refreshTimer.stop();
        }
        return;
    }
#endif

uint16_t  WaterLevel::read()
{		    
    this->level = digitalRead(this->gpio);
    if(this->level==1) return HIGH; else return LOW;    
}