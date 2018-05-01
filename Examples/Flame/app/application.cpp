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
#include "Flame.h"


void _Device();
void _Resource();

Device *device;
Flame *flame;
         
void _Device()
{	
    device->start();        
    _Resource();
}

void _Resource()
{        
    flame->setFilter(false);
    flame->start();
}

//Station connected
void connected(IPAddress ip, IPAddress mask, IPAddress gateway)
{	
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
    device = new Device("E72A6E08C7D7163230648DA2","2DCE5A8BF5171985CBAF5113");
    flame = new Flame(96,GPIO5);
    
    //Set GPIO LED RGB
    device->ledRGB->set(GPIO13,GPIO12,GPIO14,CATODO);
    device->ledRGB->color(RED);

    device->setConfig(SYS_CPU_160MHZ,NONE_SLEEP_T,SYS_FILES_DISABLED,DEBUG_ENABLED,COMMAND_DISABLED);
    device->setNetworkConfig("Morse","igor240793");
}  