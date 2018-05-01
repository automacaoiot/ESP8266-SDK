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

#include "Hcsr501.h"

#ifdef WIFI
    int Hcsr501::counterHcsr501 = 0;
   
    Hcsr501::Hcsr501(GpiosEnum gpio, unsigned long refreshHcsr501,String nameHCSR501)
    {
        resourceJson = "Hcsr501"+String(counterHcsr501++);
        resourcesJson[indexJson++] = resourceJson;
        counterJson = counterHcsr501;
        this->gpio = gpio;
        refresh = refreshHcsr501;
        name = nameHCSR501+String(counterHcsr501);
        resourceWifiMode = true;   
    }
#endif        
    
Hcsr501::Hcsr501(unsigned long id, GpiosEnum gpio, unsigned long refreshHcsr501,String nameHCSR501)
{
    this->id = id;
    this->gpio = gpio;
    refresh = refreshHcsr501;
    name = nameHCSR501+id;
    resourceWifiMode = false;    
}
            
void ICACHE_FLASH_ATTR Hcsr501::setModeConfigNetwork()
{
    #ifdef WIFI
        if(selectModeNetwork()==1) set(this->gpio,refresh); else if(selectModeNetwork()==2) set(this->id,this->gpio,refresh);       
    #else
        set(this->id,this->gpio,refresh);
    #endif  
    return;
}

void ICACHE_FLASH_ATTR Hcsr501::set(unsigned long id, GpiosEnum gpio, unsigned long refresh)
{	
    resourceWifi = true;
    pinMode(gpio, INPUT);
    return;
}

#ifdef WIFI
    void ICACHE_FLASH_ATTR Hcsr501::set(GpiosEnum gpio, unsigned long refresh)
    {
        this->id = idResource(resourceJson);
        if(this->id!=0) set(this->id,gpio,refresh); else 
        {
            resourceWifi = false;
            debugIOT(MSG_ERROR,"Resource Hcsr501 referenced in main was not sent by JSON");
            refreshTimer.stop();
        }
        return;	
    }
#endif

void ICACHE_FLASH_ATTR Hcsr501::responseHttpCallback()
{
    if(!systemCall) {
        read();
        refreshTimer.start();
    }
    return;
}

void ICACHE_FLASH_ATTR Hcsr501::actionStart()
{
    if((clientDeviceHTTP!=200) ||(clientHTTP->code==403)) {
        serverConfig->url = deviceConfig.server+"/api/device/"+deviceConfig.publicKey+"/resource/"+String(this->id)+"/feeds/last";
        sendHTTP(BLANK,GET);
    }
    
    if(RequestQueueOn) {
        String heap = String(system_get_free_heap_size());
        debugIOT(MSG_INFO,name+" -  Memory HEAP",heap);  

        if(filter) {
            if(this->hcsr501Presence != json->response->feed.rawData.toInt()) {
                doCallback(CallbackEvents::DATA_CHANGED);
                serverConfig->url = deviceConfig.server+"/api/device/"+deviceConfig.publicKey+"/resource/"+String(this->id)+"/feeds";
                sendHTTP(json->parseJson(this->hcsr501Presence),POST);
            }
            else {
                serverConfig->url = deviceConfig.server+"/api/device/"+deviceConfig.publicKey+"/resource/"+String(this->id)+"/feeds/last";
                sendHTTP(BLANK,GET);
            }
        } else {
            doCallback(CallbackEvents::DATA_CHANGED);
            serverConfig->url = deviceConfig.server+"/api/device/"+deviceConfig.publicKey+"/resource/"+String(this->id)+"/feeds";
            sendHTTP(json->parseJson(this->hcsr501Presence),POST);
        }
    }
    return;
}

unsigned long ICACHE_FLASH_ATTR Hcsr501::read()
{	
    this->hcsr501Presence = digitalRead(this->gpio);
    return this->hcsr501Presence;
}