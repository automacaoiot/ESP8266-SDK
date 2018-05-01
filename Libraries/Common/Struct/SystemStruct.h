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

#ifndef SYSTEM_STRUC_H
#define SYSTEM_STRUC_H

/**
 * Structure ClientHttp
 * @param raw Http
 * @param body Http
 * @param code Http
 * @param getHeader Http
 * @param date Http
 * @param stateTCP 
*/
struct ClientHttp
{
    String raw;
    String body;
    int code;
    String getHeader;
    DateTime date;
    TcpClientState stateTCP;	    
};

/**
 * Structure ServerConfig
 * @param url server
 * @param json
*/
struct ServerConfig
{
    String url;
    String json;            
};

/**
 * Structure DeviceConfig
 * @param publicKey device
 * @param secretKey device
 * @param server
*/
struct DeviceConfig
{
    String publicKey;
    String secretKey;
    String server;
};

/**
 * Structure NetworkConfig
 * @param staSSID SSID network
 * @param staPassword network
*/
struct NetworkConfig
{
    String staSSID;
    String staPassword;
};

/**
 * Structure LogConfig
 * @param code Esp8266
 * @param message description code
*/
struct LogConfig
{
    String code;
    String message;
};

/**
 * Structure Gadget
 * @param id_device 
 * @param id_user 
 * @param lifetime presence device
 * @param lifetimeUpdatedAt presence device
 * @param id_syscall_feed
 * @param id_syscall_type
 * @param expires_at
 * @param executed_at
*/
struct Gadget
{   
    unsigned long id_device;
    unsigned long id_user;
    int lifetime;
    String lifetimeUpdatedAt;
    unsigned long id_syscall_feed;
    unsigned long id_syscall_type;
    DateTime expires_at;
    DateTime executed_at;
};

/**
 * Structure Resource
 * @param idResource
 * @param name 
 * @param description
 * @param lifetime presence resource
 * @param lifetimeUpdatedAt presence resource
 * @param active
 * @param isAlive
*/
struct Resource
{    
    unsigned long idResource;
    String name;
    String description;
    int lifetime;
    String lifetimeUpdatedAt;
    bool active;
    bool isAlive;    
};

/**
 * Structure Feed
 * @param idResourceFeed
 * @param idLastFeed
 * @param rawData
*/
struct Feed
{
    unsigned long idResourceFeed;
    unsigned long idLastFeed;   
    String rawData;    
};

/**
 * Structure Log
 * @param code
 * @param message
 * @param idDeviceLog
*/
struct Log
{
    String code;
    String message;
    unsigned long idDeviceLog;    
};

/**
 * Structure Subtype
 * @param idResourceSubtype
 * @param idResourceType
 * @param name
 * @param nameI18n
 * @param description
 * @param dataFormatOperation
 * @param dataRestrictionRegex
 * @param dataRestrictionBeginRange
 * @param dataRestrictionEndRange
*/
struct Subtype
{
    unsigned long idResourceSubtype;
    unsigned long idResourceType;
    String name;
    String nameI18n;
    String description;
    int dataFormatOperation;
    String dataRestrictionRegex;
    unsigned long dataRestrictionBeginRange;
    unsigned long dataRestrictionEndRange;
};

/**
 * Structure Response
 * @param success
 * @param message
 * @param payload
 * @param device
 * @param feed
 * @param resource
 * @param log
 * @param subtype
*/
struct Response
{	
    bool success;        
    String message;	
    String payload;
    Gadget device;
    Feed feed;
    Resource resource;
    Log log;
    Subtype subtype;
};

#endif //SYSTEM_STRUC_H
