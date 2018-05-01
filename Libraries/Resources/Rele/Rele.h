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

#ifndef INCLUDE_RELE_H_
#define INCLUDE_RELE_H_

#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include "BaseDevice.h"

/**
 * Class Rele Set up and executing the resource
 *
 */
class Rele : public BaseDevice
{
	private:
                        
            GpiosEnum gpio;
            int status;     
            int contato;

            #ifdef WIFI
                static int counterRele;            

                /**
                * Set the resource over the WIFI network            
                * @param gpio GPIO associated with the resource
                * @param refresh Resource timer update in milliseconds
                */
                void set(GpiosEnum gpio, unsigned long refreshRele);                
            #endif

            /**
             * Set the resource manually
             * @param id Resource
             * @param gpio Associated with the resource
             * @param refresh Resource timer update in milliseconds
             */                
            void set(unsigned long id, GpiosEnum gpio, unsigned long refreshRele);
            
            /**
             * Check status change of the resource
             * @param status HIGH = 1 ou LOW = 0
             */    
            void state(uint16_t status);
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
                        
	protected:		
		
	public:

            Rele();
            Rele(unsigned long id, GpiosEnum gpio, ContatoReleEnum contato=RELE_OPEN, unsigned long refreshRele=1000, String nameRELE="Rele");
            Rele(GpiosEnum gpio, ContatoReleEnum contato=RELE_OPEN, unsigned long refreshRele=1000,String nameRELE="Rele");
            virtual ~Rele(){};
                
            /**
             * Reads JSON received from IOT database through API
             * @return RAW_DATA
             */
            uint16_t read();            
};
#endif