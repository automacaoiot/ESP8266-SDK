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

#ifndef INCLUDE_BASE_DEVICE_H_
#define INCLUDE_BASE_DEVICE_H_

#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include "RequestQueue.h"
#include "CallbackEvents.h"
#include "Config.h"
#include "../Enum/ResourceEnum.h"
#include "../Enum/SystemEnum.h"
#include "../Struct/SystemStruct.h"
#include "Json.h"
#include "LedRGB.h"


/**
 * Class Base Device Set up and executing the Base Device
 */
class BaseDevice //: public WIFI_CONFIG
{               
        //typedef void (*CallBack)(BASE_DEVICE*);
        
	private:
                        
            const String LOG_CONFIG_FILE = ".log.conf"; /**< Data file Log */
            const String IOTWIFI_CONFIG_FILE = ".iotwifi.conf"; /**< Data file Wifi */            
            
            /**
             * Performs action through the timer
             */            
            virtual void actionStart();	   
            /**
             * Perform resource action after receiving callback
             */
            virtual void responseHttpCallback();
            /**
             * set mode Config Network 
             * WIFI or Normal
             */
            virtual void setModeConfigNetwork();
            /**
              * Reads device status success and change color led RGB
              * success = true color GREEN
              * success = false up to 10 consecutive times color YELLOW
              * success = false more than 10 consecutive times color RED
              */
            virtual void presence();
            
	protected:	
            
            Timer refreshTimer; /**< Base Device timer */
            
	public:
                        
            typedef void (*CallBack)(BaseDevice*);
            
            BaseDevice(){};                
            virtual ~BaseDevice(){};      
                        
            LogConfig * logConfig = new LogConfig; 
            Json * json = new Json(); 
            LedRGB *ledRGB= new LedRGB();
            ServerConfig * serverConfig = new ServerConfig;
            HttpClient * httpClient = new HttpClient;
            ClientHttp * clientHTTP = new ClientHttp;
            CallBack * callBack = NULL;            
            
            static RequestQueue requestQueue;
            static DeviceConfig deviceConfig;
            static NetworkConfig networkConfig;
            static bool RequestQueueOn;
            static bool filter;
            static bool deviceWifiMode;
            static bool systemCall;
            static int switchSystemCall;
            static int clientDeviceHTTP;
            bool resourceWifi;
            bool resourceWifiMode;
            static bool debugMode;
            const String BLANK = ""; /**< Define variavel nula para ser utilizado no sendHTTP*/
            String name;
            unsigned long refresh;
            unsigned long id;
            
            #ifdef WIFI
                static String resourcesJson[MAX_RESOURCES];
                static String subresourcesJson[MAX_RESOURCES][MAX_SUBRESOURCES];            
                static int indexJson;
                long int idr[10];
                int counterJson;
                String resourceJson;
                        
                /**
                 * Read ID Resource over the WIFI network
                 */                              
                long int idResource(String name);
                /**
                 * Read IDs Resource over the WIFI network
                 */                                
                void idsResource(String name);	          
            #endif
            /**
             * Delete file 
             * @param type of file Log or Wifi
             */                
            void deleteFile(TypeFileEnum type);
            /**
             * Check if the file exists
             * @param type of file Log or Wifi
             * @return True file exists or False not exists
             */
            bool existFile(TypeFileEnum type);
            /**
             * Load File
             * @param type of file Log or Wifi
             * @return data of file
             */
            String loadFile(TypeFileEnum type);
            /**
             * Save File
             * @param type of file Log or Wifi
             * @param buffer data to write in the file
             */
            void saveFile(TypeFileEnum type, String buffer="");     
            /**
             * Filter TRUE will not send repeated sensor data
             * @param filter
             * @return True enable filter or False disable filter
             */
            void setFilter(bool filter=false);
            /**
             * Stop timer of device e resources
             */
            virtual void stop();               
            /**
             * Rotutine setcallback
             * @param callBackEvents
             * @param callBack
             */            
            void setCallBack(CallbackEvents event, CallBack callBack);
            /**
             * Execute callback event
             */
            void doCallback(CallbackEvents event);
            /**
             * Initializes the resource timer
            */
            void start();
            /**
             * Send Json through the API to the IOT database
             * @param Json
             */            
            void sendHTTP(String Json, HttpMethodsEnum method=GET);
            /**
             * Gets Json Back
             * @param client
             * @param successful
             */
            int callBackHttp(HttpClient& client, bool successful);
            /**
             * modeNetwork - Verifies that the device and features are configured in the same way, 
             *               as well as the call of the setNetwork member
             * @return 1 config is WIFI, 2 config is manually or 3 Error
             */
            int selectModeNetwork();            
            /**
             * Show Message Error, Warning or Info
             * @param Message Type
             * @param Message
             * @param param01
             * @param param02
             */
            void debugIOT(TypeMessageEnum messsageType, String message , String param01="",String param02="",String param03="");
            /**
             * Clean structures
            */
            void cleanResponse();
};
#endif