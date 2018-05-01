//
// Created by Igor on 02/01/2018.
//

#ifndef REQUEST_QUEUE_HTTPCLIENTQUEUE_H
#define REQUEST_QUEUE_HTTPCLIENTQUEUE_H

#include <SmingCore/SmingCore.h>
#include <queue>
#include "Request.h"
#include "../Enum/RequestStatusEnum.h"

using namespace std;

class RequestQueue {

    struct Queue{
        
        Request * request;
        Queue * next;
    };

    public:

        RequestQueue();

        int size;

        const int maxRequests = 15;
        const int executionTimeMs = 50;
    
        int addQueue(Request * request);
        
        void clean();
        
        bool isHttpProcessing();
        
        bool isRequestBusy(unsigned long id);
        
        bool isRequestInQueue(unsigned long id);
        
        bool isQueueFull();
        
        bool isQueueEmpty();
        
        RequestStatusEnum getRequestStatus(unsigned long id);
        
    private:
    
        HashMap<unsigned long, RequestStatusEnum> requestStatus;
                
        HttpClient * httpClient;
    
        Timer executionTimer;
    
        Queue * nextInQueue = NULL;
        Queue * lastInQueue = NULL;
    
        HttpClientCompletedDelegate internalHttpCompletedCallBack = HttpClientCompletedDelegate(&RequestQueue::httpCompletedCallBack,this);
        
        TimerDelegate queueExecutionDelegate = TimerDelegate(&RequestQueue::executeQueue, this);

        void httpCompletedCallBack(HttpClient &httpClient, bool success);
        
        void executeQueue();
        
        void push(Request * request);
        
        void pop();
            
};


#endif //REQUEST_QUEUE_HTTPCLIENTQUEUE_H
