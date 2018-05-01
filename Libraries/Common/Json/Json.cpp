#include "Json.h"

bool   Json::containsNestedKey(const JsonObject& obj, const char* key) {
    for (const JsonPair& pair : obj) {
        if (!strcmp(pair.key, key))
            return true;

        if (containsNestedKey(pair.value.as<JsonObject>(), key)) 
            return true;
    }

    return false;
}

void   Json::readResponse(String buffer)
{
    DynamicJsonBuffer jsonBuffer(sizeof(buffer));
            
    //Converte String to Char*
    char *Json = new char[buffer.length()+1];
    memcpy(Json, buffer.c_str(), buffer.length() + 1);

    JsonObject& root = jsonBuffer.parseObject(Json);
        
    if (root.success())    
    {        
        //Parse Sucesso
        if(root.containsKey("success")) response->success = root["success"]; //else response->success=0;
        if(root.containsKey("message")) response->message = root["message"].as<String>();//response->message="Erro Message";
        if(root.containsKey("payload")) response->payload = root["payload"].as<String>();//response->payload="";
               
        JsonObject& payload = root["payload"];
  
        if(payload.containsKey("raw_data")) response->feed.rawData = payload["raw_data"].as<String>();
        if(payload.containsKey("id_resource_feed")) response->feed.idResourceFeed = payload["id_resource_feed"].as<int>();
        if(payload.containsKey("id_last_feed")) response->feed.idLastFeed = payload["id_last_feed"].as<int>();
        
        // Device
        if(payload.containsKey("lifetime")) response->device.lifetime = payload["lifetime"].as<int>();
        if(payload.containsKey("lifetime_updated_at")) response->device.lifetimeUpdatedAt = payload["lifetime_updated_at"].as<String>();
        
        JsonObject& device = payload["last_sys_call"];
        
        if(device.success()) {
            if(device.containsKey("id_syscall_feed")) response->device.id_syscall_feed = device["id_syscall_feed"].as<int>();
            if(device.containsKey("id_syscall_type")) response->device.id_syscall_type = device["id_syscall_type"].as<int>();
            if(device.containsKey("expires_at")) response->device.expires_at = device["expires_at"].as<int>();
            if(device.containsKey("executed_at")) response->device.executed_at = device["executed_at"].as<int>();                
        } else {
            response->device.id_syscall_feed = 0;
            response->device.id_syscall_type = 0;
            response->device.expires_at = 0;
            response->device.executed_at = 0;            
        }        
    }
    
    free(Json);         
    return;
}

String   Json::parseJson()
{
    String JsonOUT;
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& Json = jsonBuffer.createObject();
    Json["success"] = "false";
    Json["payload"] = BLANK;
    Json["message"] = BLANK;
    
    Json.printTo(JsonOUT);
    return(JsonOUT);
}

String   Json::parseJson(float buffer)
{
    String JsonOUT;
    DynamicJsonBuffer jsonBuffer(sizeof(buffer));
    
    JsonObject& Json = jsonBuffer.createObject();
    Json["input"] = buffer;
    
    Json.printTo(JsonOUT);
    return(JsonOUT);
}

String  Json::parseJson(String buffer)
{
    String JsonOUT;
    DynamicJsonBuffer jsonBuffer(sizeof(buffer));
    //StaticJsonBuffer<200> jsonBuffer;

    JsonObject& Json = jsonBuffer.createObject();
    Json["input"] = buffer;
    
    Json.printTo(JsonOUT);
    return(JsonOUT);
}

String  Json::parseJson(int buffer)
{
    String JsonOUT;
    DynamicJsonBuffer jsonBuffer(sizeof(buffer));
    
    JsonObject& Json = jsonBuffer.createObject();
    Json["input"] = buffer;
        
    Json.printTo(JsonOUT);
    return(JsonOUT);
}

String  Json::parseJson(long int buffer)
{
    String JsonOUT;
    DynamicJsonBuffer jsonBuffer(sizeof(buffer));
    
    JsonObject& Json = jsonBuffer.createObject();
    Json["input"] = buffer;
        
    Json.printTo(JsonOUT);
    return(JsonOUT);
}

String  Json::parseJson(unsigned long buffer)
{
    String JsonOUT;
    DynamicJsonBuffer jsonBuffer(sizeof(buffer));
    
    JsonObject& Json = jsonBuffer.createObject();
    Json["input"] = buffer;
        
    Json.printTo(JsonOUT);
    return(JsonOUT);
}

String  Json::parseJson(String buffer [30][10])
{
    String JsonOUT;
    DynamicJsonBuffer jsonBuffer(sizeof(buffer[30][10]));
    
    JsonArray& data = jsonBuffer.createArray();
    
    for ( uint16_t i = 0; i < 30; i++ )
    {
        if (buffer[i][0]==0) break; else
        {
            JsonObject& Json = data.createNestedObject();
            Json["id_resource"] = buffer[i][0];
            Json["input"] = buffer[i][1];
        }
    }
    
    data.printTo(JsonOUT);
    return(JsonOUT);
}

String Json::parseJson(unsigned long buffer [30][10])
{
    String JsonOUT;
    DynamicJsonBuffer jsonBuffer(sizeof(buffer[30][10]));

    JsonArray& data = jsonBuffer.createArray();
	
    for ( uint16_t i = 0; i < 30; i++ )
    {
	    if (buffer[i][0]==0) break; else
	    {
            JsonObject& Json = data.createNestedObject();                        
            Json["id_resource"] = buffer[i][0];
            Json["input"] = buffer[i][1];
	    }
    }
    
    data.printTo(JsonOUT);
    return(JsonOUT);
}

String  Json::parseJson(int buffer [30][10])
{
    String JsonOUT;
    DynamicJsonBuffer jsonBuffer(sizeof(buffer[30][10]));

    JsonArray& data = jsonBuffer.createArray();
	
    for ( uint16_t i = 0; i < 30; i++ )
    {
	    if (buffer[i][0]==0) break; else
	    {
            JsonObject& Json = data.createNestedObject();                        
            Json["id_resource"] = buffer[i][0];
            Json["input"] = buffer[i][1];
	    }
    }
    
    data.printTo(JsonOUT);
    return(JsonOUT);
}

String  Json::parseJson(long int buffer [30][10])
{
    String JsonOUT;
    DynamicJsonBuffer jsonBuffer(sizeof(buffer[30][2]));
  
    JsonArray& data = jsonBuffer.createArray();
        
    for ( uint16_t i = 0; i < 30; i++ )
    {
        if (buffer[i][0]==0) break; else
	{
            JsonObject& Json = data.createNestedObject();                        
            Json["id_resource"] = buffer[i][0];
            Json["input"] = buffer[i][1];
	}
    }
    
    data.printTo(JsonOUT);
    return JsonOUT;	
}

String  Json::parseLogJson(String code, String msg)
{
    String JsonOUT;
    StaticJsonBuffer<200> jsonBuffer;

    JsonObject& Json = jsonBuffer.createObject();
    Json["code"] = code;
    Json["message"] = msg;	
        
    Json.printTo(JsonOUT);
    return(JsonOUT);
}