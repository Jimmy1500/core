#ifndef REQUEST_HANDLER_FACTORY_H
#define REQUEST_HANDLER_FACTORY_H

#include <Poco/Net/HTTPRequestHandlerFactory.h>

#include "Controller.h"

class RequestHandlerFactory : public HTTPRequestHandlerFactory
{
    static size_t requestCount;
    public:
        virtual HTTPRequestHandler* createRequestHandler(const HTTPServerRequest &) {
            mtx.lock(); ++requestCount; mtx.unlock();
            return new Controller(requestCount);
        }
};

size_t RequestHandlerFactory::requestCount = 0;
#endif
