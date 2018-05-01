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

#ifndef INCLUDE_WATERLEVEL_H_
#define INCLUDE_WATERLEVEL_H_

#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include "BaseDevice.h"


class WaterLevel : public BaseDevice
{
	private:
                        
            uint16_t level;
            uint16_t levelFILTER;            
            GpiosEnum gpio;

            #ifdef WIFI
                static int counterWaterLevel;
                /**
                * set the resource over the WIFI network
                * @param gpio of resource
                * @param refresh Resource timer update in milliseconds
                */
                void set(GpiosEnum gpio, unsigned long refreshWaterLevel);                
            #endif

            /**
             * set the resource manually
             * @param id of resource
             * @param gpio of resource
             * @param refresh Resource timer update in milliseconds
             */
            void set(unsigned long id, GpiosEnum gpio, unsigned long refreshWaterLevel);
            
            /**
             * Resource timer will run this rotutine
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
                        
	protected:		
		
	public:

            //WATERLEVEL();
            WaterLevel(GpiosEnum gpio, unsigned long refreshWaterLevel=1000,String nameWATERLEVEL="WaterLevel");
            WaterLevel(unsigned long id, GpiosEnum gpio, unsigned long refreshWaterLevel=1000,String nameWATERLEVEL="WaterLevel");
            ~WaterLevel(){};
                
            /**
             * Reads resource data
            */
            uint16_t read();	
};
#endif