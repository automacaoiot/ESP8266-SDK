#ifndef INCLUDE_JSON_H_
#define INCLUDE_JSON_H_

#include <user_config.h>
#include <SmingCore/SmingCore.h>
//#include "Config.h"
#include "../Struct/SystemStruct.h"

/**
 * Classe JSON - Set up parse libraries JSON
 */
class Json
{
	private:
            
            const String BLANK = ""; 

	public:		

            Response *response=NULL;
            /**
             * Retrieves Json's NestedKey received through the API
             * @param obj
             * @param key
             * @return True there is a key, False there is no key
             */            
            bool containsNestedKey(const JsonObject& obj, const char* key);            
            /**
             * Reads Json received through API
             * @param Buffer
             */
            void readResponse(String Buffer);   
            /**
             * Parse Json empty
             * @return Json 
             */
            String parseJson();
            /**
             * Parse String Json to be sent via API
             * @param buffer 
             * @return Json 
             */
            String parseJson(String buffer);		
            /**
             * Parse Float Json to be sent via API
             * @param buffer 
             * @return Json 
             */
            String parseJson(float buffer);
            /**
             * Parse Integer Json to be sent via API
             * @param buffer 
             * @return Json 
             */
            String parseJson(int buffer);
            /**
             * Parse Integer long Json to be sent via API
             * @param buffer 
             * @return Json 
             */
            String parseJson(long int buffer);		
            /**
             * Parse unsigned long Json to be sent via API
             * @param buffer 
             * @return Json 
             */
            String parseJson(unsigned long buffer);		            
            /**
             * Parse String array Json to be sent via API
             * @param buffer 
             * @return Json 
             */
            String parseJson(String buffer [30][10]);		
            /**
             * Parse Integer array Json to be sent via API
             * @param buffer 
             * @return Json 
             */
            String parseJson(int buffer [30][10]);
            /**
             * Parse Float array Json to be sent via API
             * @param buffer 
             * @return Json 
             */
            String parseJson(long int buffer [30][10]);		
            /**
             * Parse reset log Json to be sent via API
             * @param code reset esp8266
             * @param msg code reset
             * @return Json 
             */
             String parseJson(unsigned long buffer [30][10]);		
            /**
             * Parse reset log Json to be sent via API
             * @param code reset esp8266
             * @param msg code reset
             * @return Json 
             */

            String parseLogJson(String code, String msg);           
            
            Json(){
               response = new Response; 
            }

            virtual ~Json(){
                free(response);
            }
};
#endif