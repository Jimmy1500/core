#ifndef REQUEST_HANDLER_FACTORY_H
#define REQUEST_HANDLER_FACTORY_H

#include <Poco/Net/HTTPRequestHandlerFactory.h>

#include "RequestHandler.h"

class RequestHandlerFactory : public HTTPRequestHandlerFactory
{
    public:
        virtual HTTPRequestHandler* createRequestHandler(const HTTPServerRequest &) {
            return new RequestHandler;
        }
};

#endif
