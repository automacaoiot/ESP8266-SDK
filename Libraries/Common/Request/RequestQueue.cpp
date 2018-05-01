//
// Created by Igor on 02/01/2018.
//
#include "RequestQueue.h"

RequestQueue::RequestQueue() {

    this->httpClient = new HttpClient;
    this->size = 0;

}

void RequestQueue::httpCompletedCallBack(HttpClient &httpClient, bool success) {
    
    if (this->nextInQueue != NULL){
    
        Request * request = this->nextInQueue->request;
        
        RequestCompletedDelegate requestCompletedDelegate = request->getRequestCompletedDelegate();
        
        if (requestCompletedDelegate){
        
            int status = requestCompletedDelegate(httpClient, success);
        
            if (status == 0)
                this->pop();
        }
        else
            this->pop();
    }

    this->httpClient->reset();

    if (this->size > 0){
        executionTimer.initializeMs(this->executionTimeMs, this->queueExecutionDelegate).startOnce();
    }
}

void RequestQueue::executeQueue() {

    if (this->nextInQueue != NULL){
        
        Request * request = this->nextInQueue->request;
        
        const HashMap<String,String> * headers = &request->getHeaders();
        
        for (int i = 0; i < headers->count(); i++)
            this->httpClient->setRequestHeader(headers->keyAt(i), headers->valueAt(i));
	    
	    this->httpClient->setPostBody(request->getBody());
                
        Serial.println("\nRequest: " + request->getName() + " now Parsing to be Executed!\n");
        
        this->requestStatus[request->getId()] = RequestStatusEnum::PARSING;
            
        this->httpClient->downloadString(request->getUrl(), request->getHttpMethodString(), this->internalHttpCompletedCallBack);
        
        this->requestStatus[request->getId()] = RequestStatusEnum::EXECUTING;
   
    }
}

int RequestQueue::addQueue(Request * request) {
    

    if (this->size < this->maxRequests) {

        this->push(request);
    
        Serial.println("\nRequest: " + request->getName() + " Added to the Request Queue! \tQueue Size: " + String(this->size) + "/" + String(this->maxRequests) + "\n");

        if (this->size == 1)
            executionTimer.initializeMs(this->executionTimeMs, this->queueExecutionDelegate).startOnce();
        
        return 0;
    }
    
    delete request;

    return 1;
}

void RequestQueue::clean(){
    
    Serial.println("\n\n\tCleaning Request Queue!\n\n");
    
    if (this->executionTimer.isStarted())
        this->executionTimer.stop();
    
    while (this->nextInQueue != NULL){
        
        this->pop();
        
    }
}

bool RequestQueue::isQueueFull(){
    return this->size >= this->maxRequests;
}

bool RequestQueue::isQueueEmpty(){
    return this->size == 0;
}

bool RequestQueue::isHttpProcessing(){
    return this->httpClient->isProcessing();
}

bool RequestQueue::isRequestBusy(unsigned long id){
    
    if (this->requestStatus.contains(id))
        return this->requestStatus[id] == RequestStatusEnum::EXECUTING || this->requestStatus[id] == RequestStatusEnum::PARSING;
    
    return false;
}

bool RequestQueue::isRequestInQueue(unsigned long id){
    
    return this->requestStatus.contains(id);
    
}

RequestStatusEnum RequestQueue::getRequestStatus(unsigned long id){
    return this->requestStatus[id];
}

void RequestQueue::push(Request * request){
    
    Queue * queue = new Queue;
    
    queue->request = request;
    queue->next = NULL;
    
    if (this->size == 0){    
        this->nextInQueue = queue;
        this->lastInQueue = queue;
    }
    else{
        this->lastInQueue->next = queue;
        this->lastInQueue = queue;
    }
    
    this->size += 1;
    
    this->requestStatus[request->getId()] = RequestStatusEnum::INQUEUE;
    
}

void  RequestQueue::pop(){
    
    if (this->size > 0){
        
        Queue * oldInQueue = this->nextInQueue;
        
        this->nextInQueue = oldInQueue->next;
        
        Request * oldRequest = oldInQueue->request;
        
        Serial.println("\nRequest: " + oldRequest->getName() + " have been Popped from Queue! \tQueue Size: " + String(this->size - 1) + "/" + String(this->maxRequests) + "\n");
        
        this->requestStatus.remove(oldRequest->getId());
        
        delete oldRequest;
        delete oldInQueue;
        
        this->size -= 1;
        
        if (this->size == 1)
            this->lastInQueue = this->nextInQueue;
    }
}

