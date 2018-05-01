//
// Created by Igor on 12/01/2018.
//

#ifndef REQUEST_QUEUE_REQUEST_H
#define REQUEST_QUEUE_REQUEST_H

#include  <SmingCore/SmingCore.h>
#include "../Enum/HttpMethodsEnum.h"
//#include "../../../Sming/SmingCore/Network/HttpClient.h"

typedef Delegate<int(HttpClient& client, bool successful)> RequestCompletedDelegate;

class Request {

    public:

        Request(unsigned long id);
        Request(unsigned long id, const String name);

        HttpMethodsEnum httpMethod = HttpMethodsEnum::GET;
        unsigned long id;
        String name;
        String url;
        String body;
        HashMap<String,String> headers;
        RequestCompletedDelegate requestCompletedDelegate;

        void setName(const String name);
        void setId(unsigned long id);
        void setUrl(const String url);
        void setRequestMethod(HttpMethodsEnum httpMethodsEnum);
        void setBody(const String body);
        void setHeader(const String header, const String value);
        void setContentType(const String contentType);
        void setOnCompletedCallBack(RequestCompletedDelegate requestCompletedDelegate);

        const String &getName() const;
        const unsigned long getId() const;
        const String &getUrl() const;
        const HttpMethodsEnum &getHttpMethod() const; 
        const String getHttpMethodString() const;
        const String &getBody() const;
        const HashMap<String, String> &getHeaders() const;
        const RequestCompletedDelegate &getRequestCompletedDelegate() const;


};


#endif //REQUEST_QUEUE_REQUEST_H
