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

#include "Flame.h"

#ifdef WIFI
    int Flame::counterFlame = 0;

    Flame::Flame(GpiosEnum gpio, unsigned long refreshFlame, int gpioA,String nameFLAME)
    {   
        resourceJson = "Flame"+String(counterFlame++);
        resourcesJson[indexJson++] = resourceJson;
        counterJson = counterFlame;    
        this->gpio=gpio;
        refresh=refreshFlame;
        this->gpioA = gpioA;
        name = nameFLAME+String(counterFlame);
        resourceWifiMode = true;
    }
#endif
        
Flame::Flame(unsigned long id, GpiosEnum gpio, unsigned long refreshFlame, int gpioA,String nameFLAME)
{
    this->id = id;
    this->gpio=gpio;
    refresh=refreshFlame;
    this->gpioA = gpioA; 
    name = nameFLAME+id;
    resourceWifiMode = false;
}

void  Flame::responseHttpCallback()
{
    if(!systemCall) {
        read();    
        refreshTimer.start();
    }
    return;
}

void  Flame::actionStart()
{
    if((clientDeviceHTTP!=200) ||(clientHTTP->code==403)) {
        serverConfig->url = deviceConfig.server+"/api/device/"+deviceConfig.publicKey+"/resource/"+String(this->id)+"/feeds/last";
        sendHTTP(BLANK,GET);
        return;
    }
    
    if(RequestQueueOn) {
        String heap = String(system_get_free_heap_size());
        debugIOT(MSG_INFO,name+" -  Memory HEAP",heap);  

        if(filter)
        {
            if(lm393.presence != json->response->feed.rawData.toInt()) {   
                doCallback(CallbackEvents::DATA_CHANGED);
                serverConfig->url = deviceConfig.server+"/api/device/"+deviceConfig.publicKey+"/resource/"+String(this->id)+"/feeds";
                sendHTTP(json->parseJson(lm393.presence),POST);
	        } else {
                serverConfig->url = deviceConfig.server+"/api/device/"+deviceConfig.publicKey+"/resource/"+String(this->id)+"/feeds/last";
                sendHTTP(BLANK,GET); 
	            
	        }
        } else {
                doCallback(CallbackEvents::DATA_CHANGED);
                serverConfig->url = deviceConfig.server+"/api/device/"+deviceConfig.publicKey+"/resource/"+String(this->id)+"/feeds";
                sendHTTP(json->parseJson(lm393.presence),POST);
        }
    }
    return;
}

void  Flame::setModeConfigNetwork()
{
    #ifdef WIFI
        if(selectModeNetwork()==1) set(this->gpio,refresh,this->gpioA); else if(selectModeNetwork()==2) set(this->id,this->gpio,refresh,this->gpioA);     
    #else
        set(this->id,this->gpio,refresh,this->gpioA);
    #endif  
    return;
}

void  Flame::set(unsigned long id, GpiosEnum gpio, unsigned long refresh, int gpioA)
{  
    lm393 = {0,0,0};        
    resourceWifi = true;
    pinMode(gpio, INPUT);
    return;
}

#ifdef WIFI
    void  Flame::set(GpiosEnum gpio, unsigned long refresh, int gpioA)
    {
        this->id = idResource(resourceJson);
        if(this->id!=0)set(this->id,gpio,refresh,gpioA); else 
        { 
            resourceWifi = false;
            debugIOT(MSG_ERROR,"Resource Flame referenced in main was not sent by JSON");
            refreshTimer.stop();
        }
        return;	
    }
#endif

void  Flame::read()
{	
    lm393.presence = !(digitalRead(this->gpio));
    lm393.level = analogRead(this->gpioA);
    lm393.map = map(lm393.level,0,1024,0,100);
    return;
}