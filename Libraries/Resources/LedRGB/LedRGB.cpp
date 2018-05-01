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


#include "LedRGB.h"

LedRGB::LedRGB()
{
    this->ledActived=false;
}

void  LedRGB::color(int hue)
{
    if(this->ledActived==true)
    {
	if(this->rgbtype==0) {
            this->rgbdefault = HIGH;
            this->rgbstate = LOW;
	}
	else
	{
             this->rgbdefault = LOW;
             this->rgbstate = HIGH;
	}

    digitalWrite(this->gpio_red, this->rgbdefault);
    digitalWrite(this->gpio_green, this->rgbdefault);
	digitalWrite(this->gpio_blue, this->rgbdefault);
        //
	switch (hue) {
            case 0:
		break;
            case 1:
                digitalWrite(this->gpio_green, this->rgbstate); //GREEN
		break;
            case 2:
        	    digitalWrite(this->gpio_red, this->rgbstate);   //RED
		break;
            case 3:
		        digitalWrite(this->gpio_blue, this->rgbstate); //BLUE
		break;
            case 4:
		        digitalWrite(this->gpio_blue, this->rgbstate); // PURPLE
		        digitalWrite(this->gpio_red, this->rgbstate);
		break;
            case 5:
		        digitalWrite(this->gpio_green, this->rgbstate); // YELLOW
		        digitalWrite(this->gpio_red, this->rgbstate);
		break;
            case 6:
		        digitalWrite(this->gpio_green, this->rgbstate); // CYANO
		        digitalWrite(this->gpio_blue, this->rgbstate);
		break;
            case 7:
		        digitalWrite(this->gpio_green, this->rgbstate); // WHITE
		        digitalWrite(this->gpio_red, this->rgbstate);
		        digitalWrite(this->gpio_blue, LOW);
		break;
        }
    }	
    return;
}

void  LedRGB::blink(int hue, bool actived,  int refresh)
{
    this->state=true;
    this->hue = hue;
    if(actived==true) refreshTimer.initializeMs(refresh, TimerDelegate(&LedRGB::callBackBlink, this)).start();
    else refreshTimer.stop();
}

void  LedRGB::callBackBlink()
{
    if(this->state) color(this->hue); else color(0);
    this->state=!this->state;
}

void  LedRGB::set(int led_gpio_red, int led_gpio_green, int led_gpio_blue, TypesRGBEnum rgbtype)
{   
    //PIN GPIO 
    this->gpio_red=led_gpio_red;		
    this->gpio_green=led_gpio_green;		
    this->gpio_blue=led_gpio_blue;		
    this->rgbtype = rgbtype;
    // RGB Mode
    pinMode(this->gpio_red, OUTPUT);
    pinMode(this->gpio_green, OUTPUT);	
    pinMode(this->gpio_blue, OUTPUT);
    //Enable LED
    this->ledActived=true;

    return;
}