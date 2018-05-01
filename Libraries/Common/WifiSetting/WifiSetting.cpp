#include "WifiSetting.h"

/*
WifiSetting::WifiSetting()
{
	//
}*/

void  WifiSetting::startWebServer()
{
    #ifdef WIFI
        server.listen(80);
        server.addPath("/config", HttpPathDelegate(&WifiSetting::onListResource,this));
        server.setDefaultHandler(HttpPathDelegate(&WifiSetting::onFile,this));
    #endif
    return;
}

void  WifiSetting::onListResource(HttpRequest &request, HttpResponse &response)
{
    #ifdef WIFI
	if (request.getRequestMethod() == RequestMethod::GET)
	{
		String JsonOut;

		JsonObjectStream* stream = new JsonObjectStream();
		JsonObject& root = stream->getRoot();
		JsonArray& data = root.createNestedArray("resources");

		for (int i = 0; i <=indexJson-1; i++)
		{
			JsonObject& item = data.createNestedObject();
			item["name"] = resourcesJson[i];
			
			JsonArray& rsc = item.createNestedArray("rsc");

			for (int j = 0; j <=10; j++)
			{
				if(subresourcesJson[i][j]!="") rsc.add(subresourcesJson[i][j]); else break;				
			}
		}

		root.prettyPrintTo(JsonOut);                
        debugIOT(MSG_INFO,"JSONOUT",JsonOut);
        debugIOT(MSG_INFO,"JSON Index",String(indexJson));
                
		response.setAllowCrossDomainOrigin("*");
		response.sendJsonObject(stream);
		system_get_free_heap_size();
		debugIOT(MSG_INFO,"GET successfully sent");

	}
	else
	{
		if (request.getRequestMethod() == RequestMethod::POST)
		{
			String buffer = request.getBody();

			//Converte String to Char*
			char *Json = new char[buffer.length()+1];
			memcpy(Json, buffer.c_str(), buffer.length() + 1);

			DynamicJsonBuffer jsonBuffer;
			JsonObject& root = jsonBuffer.parseObject(Json);

			if (!root.success())
			{
				debugIOT(MSG_ERROR,"parseObject() failed");
				response.badRequest();
			}
			else
			{                                
                            debugIOT(MSG_INFO,"POST received",buffer);
                            //
                            if(json->containsNestedKey(root,"device")) debugIOT(MSG_INFO,"DEVICE True"); else debugIOT(MSG_INFO,"DEVICE False");
                            if(json->containsNestedKey(root,"resources")) debugIOT(MSG_INFO,"RESOURCES True"); else debugIOT(MSG_INFO,"RESOURCES False");
                            if(root["device"]["ssid"].success()) debugIOT(MSG_INFO,"SSID True"); else debugIOT(MSG_INFO,"SSID False");
                            if(root["device"]["pwd"].success()) debugIOT(MSG_INFO,"PWD True"); else debugIOT(MSG_INFO,"PWD False");
                            if(root["device"]["publickey"].success()) debugIOT(MSG_INFO,"PUBLICKEY True"); else debugIOT(MSG_INFO,"PUBLICKEY False");
                            if(root["device"]["secretkey"].success()) debugIOT(MSG_INFO,"SECRETKEY True"); else debugIOT(MSG_INFO,"SECRETKEY False");
                            
                            // Verifica se o JSON atende ao formato corretamente
                            if ((json->containsNestedKey(root,"device")) && (json->containsNestedKey(root,"resources")))
                            {
				if((root["device"]["ssid"].success()) && (root["device"]["pwd"].success()) &&
                            	   (root["device"]["publickey"].success()) && (root["device"]["secretkey"].success()))
                                {
                                    debugIOT(MSG_INFO,"JSON format success");
                                    saveFile(IOTWIFI,buffer);
				}
				else
				{	
                                    debugIOT(MSG_INFO,"Invalid JSON format");
                                    deleteFile(IOTWIFI);
                                    response.badRequest();	
				}
                            } 
                            else
                            {
				debugIOT(MSG_INFO,"Invalid JSON format");
                deleteFile(IOTWIFI);
				response.badRequest();	
                            }
			}
			free(Json);
			system_restart();		
		}	
		else
		{
			debugIOT(MSG_INFO,"Request Method",request.getRequestMethod());
			response.forbidden();
		}
	}    
    #endif 
    return;
}

void  WifiSetting::onFile(HttpRequest &request, HttpResponse &response)
{
	String file = request.getPath();
	if (file[0] == '/')
		file = file.substring(1);

	if (file[0] == '.')
	{
		response.forbidden();
	}
	else
	{
		response.setCache(86400, true); // It's important to use cache for better performance.
		response.sendFile(file);
	}
	return;
}