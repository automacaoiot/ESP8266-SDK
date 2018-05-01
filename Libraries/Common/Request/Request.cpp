//
// Created by Igor on 12/01/2018.
//

#include "Request.h"

Request::Request(unsigned long id){

    this->id = id;
    this->name = "Request: " + id;

}

Request::Request(unsigned long id, const String name){
    
    this->id = id;
    this->name = name;
    
}

void Request::setId(unsigned long id){
    this->id = id;
}

void Request::setName(const String name){
    
    this->name = name;
}

void Request::setRequestMethod(HttpMethodsEnum httpMethodsEnum) {
    this->httpMethod = httpMethodsEnum;
}

void Request::setBody(const String body) {
    this->body = body;
}

void Request::setContentType(const String contentType) {
    this->headers["Content-Type"] = contentType;
}

void Request::setHeader(const String header, const String value) {
    this->headers[header] = value;
}

void Request::setUrl(const String url) {
    this->url = url;
}

void Request::setOnCompletedCallBack(RequestCompletedDelegate requestCompletedDelegate) {
    this->requestCompletedDelegate = requestCompletedDelegate;
}

const HttpMethodsEnum &Request::getHttpMethod() const {
    
    return this->httpMethod;
}

const String Request::getHttpMethodString() const {
    
    if (httpMethod == HttpMethodsEnum::GET)
        return "GET";
    else if (httpMethod == HttpMethodsEnum::PUT)
        return "PUT";
    else if (httpMethod == HttpMethodsEnum::POST)
        return "POST";
    else if (httpMethod == HttpMethodsEnum::DELETE)
        return "DELETE";
    else if (httpMethod == HttpMethodsEnum::PATCH)
        return "PATCH";
    else
        return "GET";
}

const String &Request::getBody() const {
    return this->body;
}

const HashMap<String, String> &Request::getHeaders() const {
    return this->headers;
}

const RequestCompletedDelegate &Request::getRequestCompletedDelegate() const {
    return this->requestCompletedDelegate;
}

const String &Request::getUrl() const {
    return this->url;
}

const String &Request::getName() const{
    return this->name;
}

const unsigned long Request::getId() const{
    return this->id;
}