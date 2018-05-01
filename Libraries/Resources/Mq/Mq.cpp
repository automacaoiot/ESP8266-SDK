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

#include "Mq.h" 

#ifdef WIFI
    int Mq::counterMq = 0;
            
    Mq::Mq(GpiosEnum gpio,TypesGasEnum gastype,unsigned long refreshMq,int gpioA, String nameMQ)
    {
        resourceJson = "Mq"+String(counterMq++);
        subresourcesJson[indexJson][0] = "Presenca";
        subresourcesJson[indexJson][1] = "Nivel";
        resourcesJson[indexJson++] = resourceJson;
        counterJson = counterMq;
        this->gpio = gpio;
        refresh = refreshMq;
        this->gastype = gastype;
        this->gpioA = gpioA;
        name = nameMQ;
        presence = 0;
        level = 0;
        resourceWifiMode = true;
    }
#endif

Mq::Mq(unsigned long idPresence,unsigned long idLevel, GpiosEnum gpio,TypesGasEnum gastype,unsigned long refreshMq,int gpioA,String nameMQ)
{
    this->idPresence=idPresence;
    this->idLevel=idLevel;
    this->gpio = gpio;
    refresh = refreshMq;
    this->gastype = gastype;
    this->gpioA = gpioA;
    name = nameMQ;
    resourceWifiMode = false;    
}

void  Mq::setModeConfigNetwork()
{
    #ifdef WIFI
        if(selectModeNetwork()==1) set(this->gpio,this->gastype,refresh,this->gpioA); else if(selectModeNetwork()==2) set(this->idPresence,this->idLevel,this->gpio,this->gastype,refresh,this->gpioA);
    #else
        set(this->idPresence,this->idLevel,this->gpio,this->gastype,refresh,this->gpioA);
    #endif  
    return;
}

void  Mq::set(unsigned long idPresence,unsigned long idLevel, GpiosEnum gpio,TypesGasEnum gastype,unsigned long refresh,int gpioA)
{
    resourceWifi = true;
    mq = {0,0};
    pinMode(gpio, INPUT);        
    return;
}

#ifdef WIFI
    void  Mq::set(GpiosEnum gpio,TypesGasEnum gastype,unsigned long refresh,int gpioA)
    {
        idsResource(resourceJson);
        if((idr[0]!=0)&&(idr[1]!=0)) {
            this->idPresence = idr[0];
            this->idLevel = idr[1];
            set(this->idPresence,this->idLevel,gpio,gastype,refresh,gpioA); 
        }
        else 
        {
            resourceWifi = false;
            debugIOT(MSG_ERROR,"Resource Mq referenced in main was not sent by JSON");
            refreshTimer.stop();
        }
        return;	
    }
#endif

void  Mq::responseHttpCallback()
{
    if(!systemCall) {
        read();
        refreshTimer.start();    
    }
    return;
}

void  Mq::actionStart()
{
    if((clientDeviceHTTP!=200) ||(clientHTTP->code==403)) {
        serverConfig->url = deviceConfig.server+"/api/device/"+deviceConfig.publicKey+"/resources/"+String(int(this->idPresence))+","+String(int(this->idLevel));            
        sendHTTP(BLANK,GET);
    }
    
    if(RequestQueueOn) {
        String heap = String(system_get_free_heap_size());
        debugIOT(MSG_INFO,"Mq -  Memory HEAP",heap);  

        if(filter) {
            if((mq.presence!=changePresence()) && (mq.level!=changeLevel())) {
                doCallback(CallbackEvents::DATA_CHANGED);
                serverConfig->url = deviceConfig.server+"/api/device/"+deviceConfig.publicKey+"/resources/feeds";
                unsigned long mount[30][10] = {{this->idPresence,mq.presence},{this->idLevel,mq.level}};
                sendHTTP(json->parseJson(mount),POST);
            } else {
                serverConfig->url = deviceConfig.server+"/api/device/"+deviceConfig.publicKey+"/resources/"+String(int(this->idPresence))+","+String(int(this->idLevel));            
                sendHTTP(BLANK,GET);
            }
        }else {
            doCallback(CallbackEvents::DATA_CHANGED);
            serverConfig->url = deviceConfig.server+"/api/device/"+deviceConfig.publicKey+"/resources/feeds";
            unsigned long mount[30][10] = {{this->idPresence,mq.presence},{this->idLevel,mq.level}};
            sendHTTP(json->parseJson(mount),POST);
        }
    }
    return;
}

void  Mq::read()
{
    mq.presence = !(digitalRead(this->gpio));
    sensorValue = analogRead(this->gpioA);
    
    if(mq.presence==0) mq.level = 0; else 
    {
        mapValue = map(sensorValue, 0, 1023, 0, 5000);
        mapPercent = ((mapValue*100)/1023)-100;
        if(mapPercent<0) mq.level=0; else mq.level=mapPercent;
        
    }
    
    debugIOT(MSG_INFO,"MAP: ",String(mapValue));  
    debugIOT(MSG_INFO,"PRESENCE: ",String(mq.presence));  
    debugIOT(MSG_INFO,"LEVEL: ",String(mq.level));  
    
    return;
}

bool Mq::changePresence()
{
    if(mq.presence!=this->presence) { 
        this->presence = mq.presence;
        return true;
    } else return false;
}

bool Mq::changeLevel()
{
    if(mq.level!=this->level) { 
        this->level = mq.level;
        return true;
    } else return false;
}