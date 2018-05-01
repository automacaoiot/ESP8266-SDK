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

#ifndef INCLUDE_BEEP_H_
#define INCLUDE_BEEP_H_

#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include "BaseDevice.h"

/**
 * Class Beep Set up and executing the resource Beep
 */
class Beep : public BaseDevice
{
    private:

        uint16_t band;	
        uint16_t disarm;        
        HardwarePWM *HW_pwm;
        GpiosEnum gpio;
        
        #ifdef WIFI
            static int counterBeep; 
            /**
            * Set Beep resource by WIFI network
            * @param gpio GPIO associated with the resource    
            * @param refresh Resource timer update in milliseconds     
            * @param band Frequency band generate the beep
            * @param disarm timer turn off beep in milliseconds            
            */
            void set(GpiosEnum gpio, unsigned long refreshBeep, uint16_t band, uint16_t disarm);	            
        #endif

        /**
         * Set Beep resource manually
         * @param id Resource
         * @param gpio GPIO associated with the resource    
         * @param refresh Resource timer update in milliseconds     
         * @param band Frequency band generate the beep
         * @param disarm timer turn off beep in milliseconds         
         */
        void set(unsigned long id, GpiosEnum gpio, unsigned long refreshBeep, uint16_t band, uint16_t disarm);
        
        /**
         * Enable Beep
         */
        void enable();
        /**
         * Disable Beep
         */
        void disable();	        
        /**
         * Reads JSON received from IOT database through API
         * @return RAW_DATA
        */
        void read();        
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
         * Performs action through the timer
         */
        virtual void actionStart();
        /**
         * Disarm timer, turn off beep
         */
        void disarmStart();
        /**
         * Initializes the disarm timer
         */
        void startDisarm();
        
    protected:		
		
        Timer disarmTimer; /**< disarm timer */

    public:
        
        Beep(GpiosEnum gpio, unsigned long refreshBeep=1000, uint16_t band=500, uint16_t disarm=5000,String nameBeep="Beep");
        Beep(unsigned long id, GpiosEnum gpio, unsigned long refreshBeep=1000, uint16_t band=500, uint16_t disarm=5000,String nameBeep="Beep");
        ~Beep(){};        
          
        /**
         * Stop beep timer
         */
        virtual void stop();
};
#endif