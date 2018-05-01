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

#ifndef INCLUDE_MQ_H_
#define INCLUDE_MQ_H_

#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include "BaseDevice.h"

/**
 * struct GAS
 */
struct MQ
{
    unsigned long presence;
    unsigned long level;
};

/**
 * Class Mq Set up and executing the resource Mq
 */
class Mq : public BaseDevice
{
	private:
            
            int gpioA;	
            TypesGasEnum gastype;
            unsigned long idPresence, idLevel,presence,level;  
            float sensorValue, mapValue,mapPercent;
            GpiosEnum gpio;

            #ifdef WIFI
                static int counterMq;
                
                /**
                * Set the resource over the WIFI network
                * @param id Resource
                * @param gpio GPIO associated with the resource                
                * @param gastype LPG = 1 CO2 = 2 SMOKE = 3
                * @param refresh Resource timer update in milliseconds 
                * @param gpioA GPIO A0 associated with the resource
                */
                void set(GpiosEnum gpio,TypesGasEnum gastype,unsigned long refreshMq,int gpioA);                                       
            #endif

            /**
             * Set the resource manually
             * @param id Resource
             * @param gpio Associated with the resource             
             * @param gastype LPG = 1 CO2 = 2 SMOKE = 3
             * @param refresh Resource timer update in milliseconds
             * @param gpioA Associated with the resource
             */
            void set(unsigned long idPresence,unsigned long idLevel, GpiosEnum gpio,TypesGasEnum gastype,unsigned long refreshMq,int gpioA);
            
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
             * Check if presence change
             * @return True or False
             */
            bool changePresence();
            /**
             * Check if level change
             * @return True or False
             */
            bool changeLevel();
                        
	protected:		
		
	public:

            Mq(GpiosEnum gpio,TypesGasEnum gastype=LPG,unsigned long refreshMq=1000,int gpioA=A0,String nameMQ="Mq");
            Mq(unsigned long idPresence,unsigned long idLevel, GpiosEnum gpio,TypesGasEnum gastype=LPG,unsigned long refreshMq=1000,int gpioA=A0,String nameMQ="Mq");
            virtual ~Mq(){};
                
            MQ mq;
            
            /**
            * Reads resource data and write to struc GAS
             */
            void read();	                        
};
#endif