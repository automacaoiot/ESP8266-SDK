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

#ifndef RESOURCE_GPIOENUM_H
#define RESOURCE_GPIOENUM_H


enum GpiosEnum
{
	GPIO0 = 0,
	GPIO1 = 1,
	GPIO2 = 2,
	GPIO3 = 3,
	GPIO4 = 4,
	GPIO5 = 5,
	GPIO6 = 6,
	GPIO7 = 7,
	GPIO8 = 8,
	GPIO9 = 9,
	GPIO10 = 10,
	GPIO11 = 11,
	GPIO12 = 12,
	GPIO13 = 13,
	GPIO14 = 14,
	GPIO15 = 15,
	GPIO16 = 16,
	GPIO17 = 17,
	GPIO18 = 18,
	GPIO19 = 19,
	GPIO20 = 20,
	GPIO99 = 99
};

enum MetricsEnum
{
	CM = 0,
	INCHES = 1
};

enum RGBcolorEnum
{
	BLANK = 0,
	GREEN = 1,
	RED = 2,	
	BLUE = 3,
	PURPLE = 4,
	YELLOW = 5,	
	CYANO = 6,
	WHITE = 7
};


enum TypesRGBEnum
{
	CATODO = 0,
	ANODO = 1
};

enum TypesDhtEnum
{
	DHT11 = 11,
	DHT21 = 21,
	DHT22 = 22,
	DHT33 = 33,
	DHT44 = 44
};

//dhtData
struct DatadhtEnum {   
    long int temperature;
    long int humidity;  
};


enum TypesGasEnum
{
	LPG = 1,
	CO2 = 2,
	SMOKE = 3
};

enum ContatoReleEnum{
    RELE_OPEN = 0,
    RELE_CLOSED = 1,
};


#endif //RESOURCE_GPIOENUM_H
