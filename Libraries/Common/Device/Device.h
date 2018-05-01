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

#ifndef INCLUDE_DEVICE_H_
#define INCLUDE_DEVICE_H_

#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include "BaseDevice.h"

#ifdef WIFI
    #include "WifiSetting.h"
#endif

#define SYS_FILES_DISABLED 0
#define SYS_FILES_ENABLED 1
#define DEBUG_DISABLED 0
#define DEBUG_ENABLED 1
#define COMMAND_DISABLED 0
#define COMMAND_ENABLED 1

typedef Delegate<void()> InterruptDelegate;
void connected(IPAddress ip, IPAddress mask, IPAddress gateway);
void connectionFailed(String ssid, uint8_t ssidLength, uint8_t *bssid, uint8_t reason);

/**
 * Class Device Set up and executing the Device
 */
class Device : public BaseDevice
{
	private:

            bool setConf;                
            bool firstTimeDevice;
            bool systemFileDevice;
            unsigned long id;
            GpiosEnum gpio;  
            int statusCount;
                        
            #ifdef WIFI
                WifiSetting *wifiConfig= new WifiSetting();  
                /**
                 * Set the device over the WIFI network
                 */
                void set();
                /**
                 * Set the network over the WIFI network
                 */                
                void network();	                                            
            #endif
            /*
             * Performs action through the timer
             */                        
            virtual void actionStart();    
            /**
             * Perform resource action after receiving callback
            */
            virtual void responseHttpCallback();
            /*
             * Reset Device and Wifi Config  by Interrupt Handler Delegate or Syscall
             */            
            void resetDeviceWifi();
            /*
             * Reset Device by Syscall
             */            
            void resetDevice();            
            /**
             * Read code and message reset write struct LogConfig
             */
            void codeReset();	            
            /**
             * Execute action Syscall
             */
            void actionSystemCall();	
            /**
             * Send API System Call
             */
            void callerSystemCall();	
            /**
             * Verify if id_syscall_type have action to execute
             * @return True id_syscall_type have action or False id_syscall_type no have action
             */
            bool verifyActionSystemCall();
            /**
              * Reads device status success and change color led RGB
              * success = true color GREEN
              * success = false up to timelife consecutive times color YELLOW
              * success = false more than timelife consecutive times color RED
              */
            void presence(bool mode=false);

	protected:		
	
	    Timer systemCallTimer;

	public:

            Device(unsigned long refreshDevice=1000,String Server="http://automacao-iot.com.br");
            Device(String PublicKey, String SecretKey,unsigned long refreshDevice=1000, String Server="http://automacao-iot.com.br");
            virtual ~Device(){};
            
            #ifdef WIFI
                /**
                * Configure network over the WIFI network
                */
                void setNetworkConfig();
            #endif
            
            /**
             * Configure network manually
             * @param SSID
             * @param PWD
             */            
            void setNetworkConfig(String SSID, String PWD);
            /**
             * Set reset device 
             * @param gpio to LOW
             */
            void setReset(GpiosEnum gpio,uint8_t mode=FALLING);            
            /**
             * setConfig Configure parameters initialization ESP8266
             * @param freq
             * @param type
             * @param systemFile
             * @param systemDebug
             * @param command
             */
            void setConfig(uint8 freq=SYS_CPU_160MHZ,sleep_type type=NONE_SLEEP_T,bool systemFile=true, bool systemDebug=true,bool command=false);
            /**
             * Set Mode to operation automatic or manual configuration
             * @param FALSE manual mode or TRUE automatic mode
             */
            void setMODE(bool mode=false); 
};
#endif