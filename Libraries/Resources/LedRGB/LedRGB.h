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

#ifndef INCLUDE_LEDRGB_H_
#define INCLUDE_LEDRGB_H_

#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include "../../Common/Enum/ResourceEnum.h"

class LedRGB
{
	private:

            bool ledActived;
            int gpio_red;
            int gpio_green;
            int gpio_blue;
            int rgbtype;
            bool state;
            bool rgbstate;
            bool rgbdefault;
            int hue;

	protected:		
		
            Timer refreshTimer; /**< Resource timer */

	public : 

            LedRGB();
            virtual ~LedRGB(){};
            
            /**
             * Set RGB color
             * @param hue
             */
            void color(int hue);
            /**
             * Set the resource manually
             * @param led_gpio_red
             * @param led_gpio_green
             * @param led_gpio_blue
             * @param rgbtype             
             */
            void set(int led_gpio_red, int led_gpio_green, int led_gpio_blue,TypesRGBEnum rgbtype);
            /**
             * Set Blink RGB
             * @param hue
             * @param actived
             * @param refresh
             */
            void blink(int hue, bool actived=true, int refresh=1000);
            /**
             * Call Back Blink RGB
             */
            void callBackBlink();
};

#endif
