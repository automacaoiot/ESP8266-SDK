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
 
#ifndef SYSTEM_ENUM_H
#define SYSTEM_ENUM_H

enum TypeFileEnum {
    IOT = 0,
    LOG = 1,
	IOTWIFI = 2, 
};


enum TypeStateEnum{
    STATE_GREEN = 0,
    STATE_YELLOW = 1,
    STATE_RED = 2,    
};

enum TypeMessageEnum{
    MSG_ERROR = 0,
    MSG_INFO = 1,
    MSG_WARNING = 2,
};

enum SyscallEnum{
    RESET = 1,
    RESET_WIFI = 2,
};


#endif //SYSTEM_ENUM_H