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

#include "Beep.h" 

#ifdef WIFI
    int Beep::counterBeep = 0;
          
    Beep::Beep(GpiosEnum gpio, unsigned long refreshBeep, uint16_t band, uint16_t disarm,String nameBeep)
    {
        resourceJson = "Beep"+String(counterBeep++);
        resourcesJson[indexJson++] = resourceJson;
        counterJson = counterBeep;
        this->gpio = gpio;
        this->band = band;
        this->disarm = disarm;
        name = nameBeep+String(counterBeep);
        refresh = refreshBeep;
        resourceWifiMode=true;
    }
#endif    

Beep::Beep(unsigned long id, GpiosEnum gpio, unsigned long refreshBeep, uint16_t band, uint16_t disarm,String nameBeep)
{
    this->id = id;
    this->gpio = gpio;
    this->band = band;
    this->disarm = disarm;
    name = nameBeep;
    refresh = refreshBeep;
    resourceWifiMode = false;
}        

void  Beep::startDisarm()
{
    disarmTimer.initializeMs(this->disarm, TimerDelegate(&Beep::disable, this)).startOnce();
    return;
}

void  Beep::stop()
{
    disable();
    return;
}

void  Beep::responseHttpCallback()
{
    if(!systemCall) {
        read(); 
        refreshTimer.start();
    }
    return;
}

void  Beep::actionStart()
{
    if(RequestQueueOn) {
        String heap = String(system_get_free_heap_size());
        debugIOT(MSG_INFO,name+" -  Memory HEAP",heap);  
        sendHTTP(BLANK,GET);
    }
    return;
}

void  Beep::setModeConfigNetwork()
{
    #ifdef WIFI
        if(selectModeNetwork()==1) set(this->gpio,refresh,this->band,this->disarm); else if(selectModeNetwork()==2) set(this->id,this->gpio,refresh,this->band,this->disarm); 
    #else
        set(this->id,this->gpio,refresh,this->band,this->disarm);
    #endif  
    return;
}

void  Beep::set(unsigned long id,GpiosEnum gpio,unsigned long refresh, uint16_t band, uint16_t disarm)
{            
    resourceWifi = true;
    serverConfig->url = deviceConfig.server+"/api/device/"+deviceConfig.publicKey+"/resource/"+String(id)+"/feeds/last";
    pinMode(gpio, OUTPUT);
    uint8_t pin[1] = {gpio};
    this->HW_pwm = new HardwarePWM(pin,1);
    return;
}

#ifdef WIFI
    void  Beep::set(GpiosEnum gpio,unsigned long refresh, uint16_t band, uint16_t disarm)
    {
        this->id = idResource(resourceJson);
        if(this->id!=0) set(this->id,gpio,refresh,band,disarm); else 
        { 
            resourceWifi = false;
            debugIOT(MSG_ERROR,"Resource Beep referenced in main was not sent by JSON");
            refreshTimer.stop();
        }
        return;
    }
#endif

void  Beep::read()
{
    if(clientHTTP->code==200) {
        if(json->response->feed.rawData.toInt()==1) 
        {
            enable();         
            disarmTimer.start();
        } else  
        {
            refreshTimer.stop();     
            disarmTimer.stop();
            disable();
        }
    }else {
        refreshTimer.stop();     
        disarmTimer.stop();
        disable();
    }
    return;
}

void  Beep::enable()
{
    this->HW_pwm->analogWrite(this->gpio,this->band);   
    return;
}

void  Beep::disable()
{   
    HW_pwm->analogWrite(this->gpio,0);
    refreshTimer.start();
    return;
}