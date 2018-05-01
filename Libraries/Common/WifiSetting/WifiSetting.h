#ifndef INCLUDE_WIFISETTING_H_
#define INCLUDE_WIFISETTING_H_

#include <user_config.h>
#include <SmingCore/SmingCore.h>
#include "BaseDevice.h"

//#include "Json.h"


class WifiSetting : public BaseDevice
{
	private:

		HttpServer server;
                                
                /**
                 * Method GET send JSON with name and id of the resources
                 * Method POST receive JSON with configuration device and network 
                 * @param request method GET or POST
                 * @param response reply to server
                 */
		void onListResource(HttpRequest &request, HttpResponse &response);
                /**
                 * Set Webserver
                 * @param request
                 * @param response
                 */
		void onFile(HttpRequest &request, HttpResponse &response);
				
	public:		

		WifiSetting(){}
		virtual ~WifiSetting(){}
                
                /**
                 * Start Web Server
                 */
		void startWebServer();
};
#endif