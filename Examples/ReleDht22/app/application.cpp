/**
 *MIT License
 * 
 * Copyright (c) 2017 Automação-IOT
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

#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include "Device.h"
#include "Rele.h"
#include "Dht.h"

Device *device;
Rele *rele;
Dht *dht22;

void _Device();
void _Resource();

void _Device()
{	
    device->start();
    _Resource();
}

void _Resource()
{            
    // RELE
    rele->start();
    
    // DHT    
    dht22->setFilter(false);
    dht22->start();             
}

//Station connected
void connected(IPAddress ip, IPAddress mask, IPAddress gateway)
{
    device->ledRGB->color(GREEN);	
    debugf("Connected");	
    _Device();
}

// Connection failed
void connectionFailed(String ssid, uint8_t ssidLength, uint8_t *bssid, uint8_t reason)
{
    debugf("Not Connected");
    system_restart();
}

void init()
{
    //CLASSES
    device=new Device("CFD2EC542D14BBB33902D4DAF53F58CB","6661F3701CC7B71DFCEC6060771C5E158D304EAB5791F1E4D929165BBE3870A7");
    rele= new Rele(2,GPIO2,RELE_CLOSED);
    dht22=new Dht(6,7,GPIO4, DHT22,20000);
    
    //Set GPIO LED RGB
    device->ledRGB->set(GPIO13,GPIO12,GPIO14,CATODO);
    device->ledRGB->color(RED);
    
    
    device->setConfig(SYS_CPU_160MHZ,NONE_SLEEP_T,SYS_FILES_DISABLED,DEBUG_DISABLED,COMMAND_DISABLED);
    device->setNetworkConfig("Morse","igor240793");   

}  