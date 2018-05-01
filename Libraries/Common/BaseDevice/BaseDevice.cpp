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

#include "BaseDevice.h"

RequestQueue BaseDevice::requestQueue;
DeviceConfig BaseDevice::deviceConfig = {};
NetworkConfig BaseDevice::networkConfig = {};

bool BaseDevice::filter=false;
bool BaseDevice::deviceWifiMode=false;
bool BaseDevice::debugMode=false;
bool BaseDevice::systemCall=false;
bool BaseDevice::RequestQueueOn=true; 
int  BaseDevice::switchSystemCall=0;
int  BaseDevice::clientDeviceHTTP=0;

#ifdef WIFI
    String BaseDevice::resourcesJson[MAX_RESOURCES];
    String BaseDevice::subresourcesJson[MAX_RESOURCES][MAX_SUBRESOURCES];
    int BaseDevice::indexJson=0;
    //int BaseDevice::indexResource=0;
#endif
    
void BaseDevice::start()
{
    if((deviceConfig.server!=this->BLANK) && (deviceConfig.publicKey!=this->BLANK) &&(deviceConfig.secretKey!=this->BLANK)) {
        setModeConfigNetwork();
        if(resourceWifi) {
            actionStart();
            refreshTimer.initializeMs(refresh, TimerDelegate(&BaseDevice::actionStart, this)).startOnce();
        }
    }
    else {
        debugIOT(MSG_ERROR,"Set device before executing the resource");
        return;
    }        
}

void  BaseDevice::actionStart()
{
    // NOT USED
    return;
}

void  BaseDevice::responseHttpCallback()
{
    // NOT USED
    return;
}

void  BaseDevice::setModeConfigNetwork()
{
    // NOT USED
    return;
}

void  BaseDevice::presence()
{
    // NOT USED
    return;
}


int  BaseDevice::selectModeNetwork()
{
    if((deviceWifiMode==true) && (resourceWifiMode==true))  return 1; else 
    if((deviceWifiMode==true) && (resourceWifiMode==false)) return 2; else
    if((deviceWifiMode==false)&& (resourceWifiMode==false)) return 2; else    
    if((deviceWifiMode==false) &&(resourceWifiMode==true)) {            
        resourceWifi = false;
        debugIOT(MSG_ERROR,"Error::Resource %s must be set to Manual mode",name.c_str());
        refreshTimer.stop();
        return 3;
    };   
    return 0;
}

void   BaseDevice::stop()
{
    refreshTimer.stop();
    return;
}

void  BaseDevice::sendHTTP(String Json, HttpMethodsEnum method)
{
    
    // Verifies if the Request Queue is Full and Prevent from Adding Multiple Entries for the Same Request.
    
    if (!this->requestQueue.isQueueFull() && !this->requestQueue.isRequestInQueue(this->id)){
        
        Request * request = new Request(this->id, this->name);

        request->setHeader("Authorization","Bearer "+ deviceConfig.secretKey);
        request->setHeader("Connection","Close");
        request->setContentType("application/json");
        request->setUrl(serverConfig->url);
        request->setRequestMethod(method);
        
        request->setOnCompletedCallBack(RequestCompletedDelegate(&BaseDevice::callBackHttp, this));

        request->setBody(Json);

        int status = this->requestQueue.addQueue(request);
        
        
        // Http Is Processing    
        // this.requestQueue.isHttpProcessing();
        
        // Is Request Processing
        // this.requestQueue.isRequestBusy(request->name);
        
        // Clean Queue
        // this.requestQueue.clean();
        
        if (status == 1)
            debugIOT(MSG_INFO,name.c_str(),"was not Added to the Queue!\t Queue Size: ", String(this->requestQueue.size) + "/" + String(this->requestQueue.maxRequests));
    }
}

int BaseDevice::callBackHttp(HttpClient& client, bool successful){  
    
    clientHTTP->raw = client.getResponseString();
    clientHTTP->code = client.getResponseCode();
    json->readResponse(clientHTTP->raw);

    if(successful)
    {         
        if (clientHTTP->raw.length() > 0){                      
            debugIOT(MSG_INFO,name.c_str(),"::Successfully received callback");
            clientHTTP->body = client.getPostBody();
            clientHTTP->date = client.getServerDate();	

            if(json->response->success==true) {
                debugIOT(MSG_INFO,name.c_str(),"::JSON Success True"); 
            } else {    
                debugIOT(MSG_ERROR,name.c_str(),"::JSON Success False"); 
            }
            debugIOT(MSG_INFO,name.c_str(),"::Callback received",clientHTTP->raw.c_str(),String(clientHTTP->code));
        } else{            
           debugIOT(MSG_ERROR,name.c_str(),"::Invalid callBack");           
        }
    }
    else
    {
        debugIOT(MSG_ERROR,name.c_str(),"::CallBack error",clientHTTP->raw,String(clientHTTP->code));
    }
    
    responseHttpCallback();
    
    return 0;
}

void BaseDevice::cleanResponse()
{
    clientHTTP->code=0;
    json->response->success=0;
}

void  BaseDevice::deleteFile(TypeFileEnum type)
{
    switch (type) {
        case LOG: 
            if (fileExist(LOG_CONFIG_FILE)) {
                fileDelete(LOG_CONFIG_FILE);
                delay(300);
            }
            break;        
        case IOTWIFI: 
            if (fileExist(IOTWIFI_CONFIG_FILE)) {
                fileDelete(IOTWIFI_CONFIG_FILE);
                delay(300);
            }
            break;                                                      
    }
    return;
}

String  BaseDevice::loadFile(TypeFileEnum type)
{
    DynamicJsonBuffer jsonBuffer;
    
    switch (type) {
        case LOG:
            if (fileExist(LOG_CONFIG_FILE))
            {
                int size = fileGetSize(LOG_CONFIG_FILE);
                char* jsonString = new char[size + 1];
                fileGetContent(LOG_CONFIG_FILE, jsonString, size + 1);
                JsonObject& root = jsonBuffer.parseObject(jsonString);

                JsonObject& network = root["network"];
                logConfig->code =  network["code"].asString();

                delete[] jsonString;
            }
            break;
        case IOTWIFI:
            if (fileExist(IOTWIFI_CONFIG_FILE))
            {
                return (fileGetContent(IOTWIFI_CONFIG_FILE));         
            }
    }
}

void  BaseDevice::saveFile(TypeFileEnum type, String buffer)
{
    DynamicJsonBuffer jsonBuffer;
    
    switch (type) {
        case LOG:
            {
                JsonObject& root = jsonBuffer.createObject();
                JsonObject& network = jsonBuffer.createObject();
                root["network"] = network;

                network["code"] = logConfig->code;

                String rootString;
                root.printTo(rootString);
                fileSetContent(LOG_CONFIG_FILE, rootString);
                break;            
            }
        case IOTWIFI:
            fileSetContent(IOTWIFI_CONFIG_FILE, buffer);
            break;
    }
    return;
}

bool  BaseDevice::existFile(TypeFileEnum type)
{
    switch (type) {
        case LOG:
            if (fileExist(LOG_CONFIG_FILE)) return true; else return false;
            break;
        case IOTWIFI:
            if (fileExist(IOTWIFI_CONFIG_FILE)) return true; else return false;
            break;            
    }
    return false;
}

#ifdef WIFI
    long int  BaseDevice::idResource(String name)
    {
        if (existFile(IOTWIFI))
        {
            //Le arquivo com JSON
            String buffer=loadFile(IOTWIFI);//LoadFileIOT();
            
            Serial.println(buffer);
            
            //Converte String to Char*
            char *Json = new char[buffer.length()+1];
            memcpy(Json, buffer.c_str(), buffer.length() + 1);
 
            DynamicJsonBuffer jsonBuffer;
            JsonObject& root = jsonBuffer.parseObject(Json);

            if (!root.success())
            {
                debugIOT(MSG_ERROR,"parseObject() failed");
                return 0;
            }
            else
            {
                JsonArray& resource = root["resources"];
                        
                for (auto& request : resource) 
                {
                    String nameJson = request["name"]; 
                    
                    if(nameJson==name){
                        this->id = request["id"];                    
                        return this->id; 
                    }
                }                
                free(Json);                
            }
        }
        return 0;
    }


    void  BaseDevice::idsResource(String name)
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

            if (!root.success())
            {
                debugIOT(MSG_ERROR,"parseObject() failed"); 
            }
            else
            {
                JsonArray& resources = root["resources"];
        
                for (auto& request : resources) 
                {
                    String nameJson = request["name"]; 
                    if(nameJson==name) {
                        idr[0] = request["id"][0];
                        idr[1] = request["id"][1];
                        idr[2] = request["id"][2];
                        idr[3] = request["id"][3];
                        idr[4] = request["id"][4];
                        idr[5] = request["id"][5];
                        idr[6] = request["id"][6];
                        idr[7] = request["id"][7];
                        idr[8] = request["id"][8];
                        idr[9] = request["id"][9];                                                
                        return;
                    }
                }
                free(Json);
                idr[0] = 0;
                idr[1] = 0;
                idr[2] = 0;
                idr[3] = 0;
                idr[4] = 0;
                idr[5] = 0;
                idr[6] = 0;
                idr[7] = 0;
                idr[8] = 0;
                idr[9] = 0;
            }
        }
        return ;
    }
#endif

void  BaseDevice::setFilter(bool filter)
{
    this->filter = filter;
    return;
}

void  BaseDevice::setCallBack(CallbackEvents event, CallBack callBack) {

    if (this->callBack == NULL){
        
        this->callBack = new CallBack[CallbackEvents::EVENTS_SIZE];
        
    }
    
    this->callBack[event] = callBack;
    return;
}
    
void  BaseDevice::doCallback(CallbackEvents event) {

    if (this->callBack != NULL) {
        
        CallBack callBack = this->callBack[event];
        
        if (callBack != NULL)
            callBack(this);
    }
    return;
}

void  BaseDevice::debugIOT(TypeMessageEnum messsageType, String message, String param01,String param02,String param03)
{    
    if(debugMode) {
        Serial.println("");
        if(messsageType==0) Serial.println("*******************   ERROR START   *******************"); else
            if(messsageType==1) Serial.println("*******************   INFO START   *******************"); else
            if(messsageType==2) Serial.println("******************* WARNING START *******************");
    
            Serial.print(message);
            Serial.print(" ");
            Serial.print(param01);
            Serial.print(" ");
            Serial.print(param02);
            Serial.print(" ");
            Serial.println(param03);

        if(messsageType==0) Serial.println("*******************   ERROR END  *******************"); else
            if(messsageType==1) Serial.println("*******************   INFO END   *******************"); else
            if(messsageType==2) Serial.println("******************* WARNING END *******************");            
        Serial.println("");
    }; 
    return;
}