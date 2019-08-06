#ifndef REQUEST_HANDLER_FACTORY_H
#define REQUEST_HANDLER_FACTORY_H

#include <Poco/Net/HTTPRequestHandlerFactory.h>

#include "Controller.h"

class RequestHandlerFactory : public HTTPRequestHandlerFactory
{
    public:
        virtual HTTPRequestHandler* createRequestHandler(const HTTPServerRequest &) {
            mtx.lock(); ++REGISTRY::handlerCount; mtx.unlock();
            return new Controller(REGISTRY::handlerCount);
        }
};

#endif
