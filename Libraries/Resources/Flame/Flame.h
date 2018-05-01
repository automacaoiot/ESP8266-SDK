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

#ifndef INCLUDE_FLAME_H_
#define INCLUDE_FLAME_H_

#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include "BaseDevice.h"

/**
 * struct LM393
 */
struct LM393
{
    unsigned long map;
    unsigned long level;
    unsigned long presence;        
};

/**
 * Class FLAME Set up and executing the resource Flame
 */
class Flame : public BaseDevice
{
	private:
	            
            int gpioA; 
            GpiosEnum gpio;

            #ifdef WIFI
                static int counterFlame;
                /**
                * Set the resource over the WIFI network
                * @param gpio GPIO associated with the resource
                * @param refresh Resource timer update in milliseconds
                * @param gpioA0 GPIO A0 associated with the resource
                */
                void set(GpiosEnum gpio, unsigned long refreshFlame, int gpioA);
            #endif
 
            /**
             * Set the resource manually
             * @param id Resource
             * @param gpio GPIO associated with the resource
             * @param refresh Resource timer update in milliseconds
             * @param gpioA GPIO A0 associated with the resource
             */
            void set(unsigned long id, GpiosEnum gpio, unsigned long refreshFlame, int gpioA);
            
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
                        
	protected:		
				
	public:

            Flame(unsigned long id, GpiosEnum gpio, unsigned long refreshFlame=1000, int gpioA=A0,String nameFLAME="Flame");
            Flame(GpiosEnum gpio, unsigned long refreshFlame=1000, int gpioA=A0,String nameFLAME="Flame");
            virtual ~Flame(){};
                 
            LM393 lm393;
            
            /**
             * Reads resource data and write to struc LM393
             */
            void read();            
};
#endif