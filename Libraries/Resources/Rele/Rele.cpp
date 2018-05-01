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

#include "Rele.h"

#ifdef WIFI
int Rele::counterRele = 0;

Rele::Rele(){}
    
Rele::Rele(GpiosEnum gpio, ContatoReleEnum contato, unsigned long refreshRele,String nameRELE)
{    
    resourceJson = "Rele"+String(counterRele++);
    resourcesJson[indexJson++] = resourceJson;
    counterJson = counterRele;    
    this->gpio = gpio;
    this->contato = contato;
    refresh = refreshRele;
    this->status = -1;
    name = nameRELE+String(counterRele);
    resourceWifiMode = true;  
}
#endif
    
Rele::Rele(unsigned long id, GpiosEnum gpio, ContatoReleEnum contato,unsigned long refreshRele,String nameRELE)
{
    this->id = id;
    this->gpio = gpio;
    refresh = refreshRele;
    this->status = -1; 
    this->contato = contato;
    name = nameRELE+id;
    resourceWifiMode = false;  
}
            

void Rele::setModeConfigNetwork()
{
    #ifdef WIFI
        if(selectModeNetwork()==1) set(this->gpio,refresh); else if(selectModeNetwork()==2) set(this->id,this->gpio,refresh);
    #else
        set(this->id,this->gpio,refresh);
    #endif  
    return;
}

void Rele::set(unsigned long id, GpiosEnum gpio, unsigned long refresh)
{	   
    state(-1);        
    resourceWifi=true;
    serverConfig->url = deviceConfig.server+"/api/device/"+deviceConfig.publicKey+"/resource/"+String(id)+"/feeds/last";
    pinMode(gpio, OUTPUT);
    digitalWrite(gpio, HIGH);
    return;
}

#ifdef WIFI
    void  Rele::set(GpiosEnum gpio, unsigned long refresh)
    {
        this->id = idResource(resourceJson);
        if(this->id!=0) set(this->id,gpio,refresh); else 
        { 
            resourceWifi = false;
            debugIOT(MSG_ERROR,"Resource Rele referenced in main was not sent by JSON");
            refreshTimer.stop();
        }
        return;
    }
#endif

void  Rele::responseHttpCallback()
{    
    if(!systemCall) {
        state(read());
        refreshTimer.start();
    }
    return;
}

void Rele::actionStart()
{
    if(RequestQueueOn) {
        String heap = String(system_get_free_heap_size());
        debugIOT(MSG_INFO,this->name+" - Memory HEAP",heap);
        sendHTTP(BLANK,GET);
    }
    return;
}

void Rele::state(uint16_t status)
{	
    if(this->status != status)
    {
        doCallback(CallbackEvents::DATA_CHANGED);
        this->status = status;
        digitalWrite(this->gpio, this->status);
    }	
    return;
}

uint16_t  Rele::read()
{	
    if(clientHTTP->code==200) {
        if(this->contato==RELE_OPEN) {
            if(json->response->feed.rawData.toInt()==1) return HIGH; else return LOW;
        } else if(this->contato==RELE_CLOSED) {
            if(json->response->feed.rawData.toInt()==0) return HIGH; else return LOW;
            }
    } else if(this->contato==RELE_OPEN) return LOW; else if(this->contato==RELE_CLOSED) return HIGH;
    
}