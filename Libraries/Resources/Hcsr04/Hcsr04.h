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

#ifndef INCLUDE_HCSR04_H_
#define INCLUDE_HCSR04_H_

#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include <Libraries/Ultrasonic/Ultrasonic.h>
#include "BaseDevice.h"

/**
 * struct Distance
 */
struct Distance
{
  unsigned long distanceCM;
  unsigned long distanceINCHES;
};

/**
 * Class Hcsr04 Set up and executing the resource Hcsr04 (Distance)
 */
class Hcsr04 : public BaseDevice, public Ultrasonic
{
	private:
            
            uint8_t unit;
            uint16_t delay;
            int durationCM;
            GpiosEnum  trigger;
            GpiosEnum  echo;
            int distanceHcsr04;   
            GpiosEnum gpio;

            #ifdef WIFI
                static int counterHcsr04;
                
                /**
                * Set the resource over the WIFI network
                * @param id Resource
                * @param trigger Gpio associated with the hcsr04 trigger
                * @param echo GPIO associated with the hcsr04 echo
                * @param refresh Resource timer update in milliseconds
                * @param delay Time in milliseconds of echo delay
                * @param unit METRIC Centimeters = 0 Inches = 1
                */
                void set(GpiosEnum trigger, GpiosEnum echo, unsigned long refreshHcsr04, uint16_t delay, uint8_t unit);
            #endif

            /**
             * Set the resource manually
             * @param id Resource
             * @param trigger GPIO associated with the hcsr04 trigger
             * @param echo Gpio associated with the hcsr04 echo
             * @param refresh Resource timer update in milliseconds
             * @param delay Time in milliseconds of echo delay
             * @param unit METRIC Centimeters = 0 Inches = 1
             */
            void set(unsigned long id,GpiosEnum trigger, GpiosEnum echo, unsigned long refreshHcsr04, uint16_t delay, uint8_t unit);
            
            /**
             * Convert microseconds to Centimeters
             * @param microseconds
             * @return Centimeters
             */    
            int microsecondsToCentimeters(int microseconds);
            /**
             * Checks whether the distance has changed
             * @return True Modified ou False Did not modify
             */
            bool changeDistance();
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

            Hcsr04(unsigned long id,GpiosEnum trigger, GpiosEnum echo, unsigned long refreshHcsr04=1000, uint16_t delay = 20000, uint8_t unit = CM, String nameHSCR04="Hcsr04");            
            Hcsr04(GpiosEnum trigger, GpiosEnum echo, unsigned long refreshHcsr04=1000, uint16_t delay = 20000, uint8_t unit = CM, String nameHSCR04="Hcsr04");
            virtual ~Hcsr04(){};
                
            Distance distance; /**< Distance structure instance */
           
            /**
            * Reads resource data and write to struc Distance
            */
            void read();            
};
#endif