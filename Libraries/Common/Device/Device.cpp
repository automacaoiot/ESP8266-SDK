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

#include "Device.h"

#ifdef WIFI
    // WIFI
    Device::Device(unsigned long refreshDevice,String Server)
    {   
        resourceWifi = true;
        name = "Device";
        this->setConf=true;
        deviceConfig.server = Server;    
        refresh=refreshDevice;
        deviceWifiMode = true;
        this->firstTimeDevice = true;
        this->gpio=GPIO99;
        statusCount=(refresh/1000);
    }
#endif    

// MANUAL
Device::Device(String PublicKey, String SecretKey, unsigned long refreshDevice, String Server)
{   
    resourceWifi = true;
    name = "Device";
    refresh=refreshDevice;
    deviceConfig.publicKey = PublicKey;
    deviceConfig.secretKey = SecretKey;
    deviceConfig.server = Server;         
    deviceWifiMode = false;
    this->firstTimeDevice = true;
    this->gpio=GPIO99;
    statusCount=(refresh/1000);
}

void  Device::responseHttpCallback()
{
    if(systemCall) {
            actionSystemCall();
    } else {
        presence(true);
        if(json->response->device.id_syscall_feed!=0) {
            switchSystemCall = json->response->device.id_syscall_type;
            if(verifyActionSystemCall()) {
                RequestQueueOn=false;
                requestQueue.clean();
                systemCall=true;
                systemCallTimer.initializeMs(2000, TimerDelegate(&Device::callerSystemCall, this)).startOnce();
            } else systemCall=false;
        }
        refreshTimer.start();
    }
}

void  Device::actionStart()
{    
    if(systemCall) return;

    cleanResponse();
    presence(false);
    
    if(this->firstTimeDevice) {         
        this->firstTimeDevice = false;
        codeReset();        
    }else{         
        serverConfig->url = deviceConfig.server+"/api/device/"+deviceConfig.publicKey+"/info";
        sendHTTP(BLANK,GET);
    }
}    

void  Device::setReset(GpiosEnum gpio, uint8_t mode)
{
    this->gpio=gpio;
    attachInterrupt(gpio,InterruptDelegate(&Device::resetDeviceWifi,this),mode);
}

void  Device::resetDeviceWifi()
{
    if(this->gpio!=GPIO99) detachInterrupt(this->gpio);
    if (existFile(IOTWIFI)) deleteFile(IOTWIFI); 
    debugIOT(MSG_INFO,"RESTART WIFI RESTART WIFI RESTART WIFI RESTART WIFI");   
    system_restart();
}

void  Device::resetDevice()
{
    debugIOT(MSG_INFO,"RESTART RESTART RESTART RESTART RESTART RESTART");   
    system_restart();
}

void  Device::presence(bool mode)
{
    if((mode) && (json->response->success==true)) statusCount=(refresh/1000); else statusCount++;
    clientDeviceHTTP = clientHTTP->code;

    if(clientHTTP->code==200) ledRGB->color(GREEN); else 
        if((clientHTTP->code==403)||(clientHTTP->code==404)) ledRGB->color(RED); else {
            if((statusCount>((json->response->device.lifetime)*2))) ledRGB->color(RED); else 
                if(clientHTTP->code==429) ledRGB->color(RED); else
                    if(statusCount>=(json->response->device.lifetime)) ledRGB->color(YELLOW);
        }
}

void  Device::codeReset()
{
    struct rst_info* info;
    info = system_get_rst_info();
    uint8_t reason=info->reason;
        
    free(info);
    
    this->logConfig->message = DESCRIPTION_RESET[reason];
    this->logConfig->code = (String(reason));     
     
    this->serverConfig->url = this->deviceConfig.server+"/api/device/"+this->deviceConfig.publicKey+"/log";
    this->serverConfig->json = json->parseLogJson(this->logConfig->code, this->logConfig->message);
    sendHTTP(this->serverConfig->json,POST);    
    return;
}

void  Device::actionSystemCall()
{
    switch (switchSystemCall) {
        case RESET: 
            resetDevice();
            break;   
                
        case RESET_WIFI: 
            resetDeviceWifi();
            break;   
          
        default:
            callerSystemCall();
    }
}

bool  Device::verifyActionSystemCall()
{
    switch (switchSystemCall) {
        case RESET: 
            return true;
            break;   
                
        case RESET_WIFI: 
            return true;
            break;   
          
        default:
            return false;
    }
}

void  Device::callerSystemCall()
{
    serverConfig->url = deviceConfig.server+"/api/device/"+deviceConfig.publicKey+"/system/call";
    sendHTTP(BLANK,PATCH);
}

void  Device::setNetworkConfig(String SSID, String PWD)
{
    name = "Device";
    if(deviceWifiMode==false) {
        if(this->setConf) setConfig();
        networkConfig.staSSID = SSID;
        networkConfig.staPassword = PWD;
        setMODE(false);
    } else {
        resourceWifi = false;
        if (existFile(IOTWIFI)) deleteFile(IOTWIFI);        
        WifiAccessPoint.enable(false);
        WifiStation.enable(false);
        debugIOT(MSG_ERROR,"Error:Device configured for wifi mode. Use setNetworkConfig()");
    }        
    return;
}

void  Device::setConfig(uint8 freq,sleep_type type, bool systemFile, bool systemDebug,bool command)
{
    this->setConf=false;
    //SET higher CPU freq & disable wifi sleep
    system_update_cpu_freq(freq);
    //Type Sleep Wifi
    wifi_set_sleep_type(type);
    //Baud Rate Default 115200
    Serial.begin(SERIAL_BAUD_RATE);
    //Mount System File
    if(systemFile) spiffs_mount(); 
    this->systemFileDevice=systemFile;
    //System Debug
    Serial.systemDebugOutput(systemDebug);
    debugMode = systemDebug;
    Serial.commandProcessing(command);      
}


void  Device::setMODE(bool mode)
{                
    if(mode==true)
    {             
        #ifdef WIFI                    
            if (existFile(IOTWIFI))
            {     
                debugIOT(MSG_INFO,"IOT configuration file exists");
                detachInterrupt(this->gpio);
                network();
                set();
                WifiStation.config(this->networkConfig.staSSID, this->networkConfig.staPassword);
                WifiStation.enable(true);
                WifiAccessPoint.enable(false);
                wifi_station_connect();                
                WifiEvents.onStationGotIP(connected);            
                WifiEvents.onStationDisconnect(connectionFailed);
                setReset(GPIO2);
            }   
            else
            {                           
                ledRGB->blink(BLUE);
                debugIOT(MSG_INFO,"IOT configuration file does not exists");	
                randomSeed(RANDOM_REG32);
                String NameWireless = "IOT"+String(random(1, 1000));
                debugIOT(MSG_INFO,"Rede IOT ",NameWireless);
                WifiStation.enable(true);		
                WifiAccessPoint.config(NameWireless, "", AUTH_OPEN,false,0,100);
                WifiAccessPoint.enable(true);    
                wifiConfig->startWebServer();                       
            }        
        #endif            
    }
    else
    {    
        deviceWifiMode = false;
        WifiAccessPoint.enable(false);
        WifiStation.enable(true);
        WifiStation.config(networkConfig.staSSID, networkConfig.staPassword);            
        WifiEvents.onStationGotIP(connected);
        WifiEvents.onStationDisconnect(connectionFailed);        
    }    
}

#ifdef WIFI
    void  Device::setNetworkConfig()
    {   
        if(deviceWifiMode==true) {
            if(!this->systemFileDevice) spiffs_mount();    
            if(this->setConf) setConfig();
            setMODE(true);
        } else {
            resourceWifi = false;
            if (existFile(IOTWIFI)) deleteFile(IOTWIFI);
            WifiStation.enable(false);
            WifiAccessPoint.enable(false);
            debugIOT(MSG_ERROR,"Error:Device configured for normal mode. Use setNetworkConfig(SSID,PWD)");
        }
        return;
    }

    void  Device::set()
    {        
        if (existFile(IOTWIFI))
        {   
            //Le arquivo com JSON
            String buffer=loadFile(IOTWIFI);  //LoadFileIOT();

            //Converte String to Char*
            char *Json = new char[buffer.length()+1];
            memcpy(Json, buffer.c_str(), buffer.length() + 1);
 
            DynamicJsonBuffer jsonBuffer;
            JsonObject& root = jsonBuffer.parseObject(Json);
 
            JsonArray& devices = root["device"];

            const char* secretkey = root["device"]["secretkey"];
            const char* publickey = root["device"]["publickey"]; 
            deviceConfig.publicKey = publickey;
            deviceConfig.secretKey = secretkey;    
            free(Json);            
        }        
    }

    void  Device::network()
    {
        if (existFile(IOTWIFI))
        {
            //Le arquivo com JSON
            String buffer=loadFile(IOTWIFI);//LoadFileIOT();
            
            //Converte String to Char*
            char *Json = new char[buffer.length()+1];
            memcpy(Json, buffer.c_str(), buffer.length() + 1);
 
            DynamicJsonBuffer jsonBuffer;
            JsonObject& root = jsonBuffer.parseObject(Json);
 
            JsonArray& devices = root["device"];

            const char* ssid = root["device"]["ssid"];
            const char* pwd = root["device"]["pwd"];  
            
            networkConfig.staSSID = ssid;
            networkConfig.staPassword = pwd;
            
            //setNetworkConfig(ssid,pwd);
            free(Json);
        }
    }
#endif