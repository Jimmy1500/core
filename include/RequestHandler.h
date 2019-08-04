#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Stringifier.h>
#include <Poco/Dynamic/Var.h>

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include "Repository.h"

using namespace std;
using namespace Poco::Net;
using namespace Poco::JSON;

typedef map<string, function<void(HTTPServerRequest& request, HTTPServerResponse& response)>> FuncMap;

class RequestHandler : public HTTPRequestHandler {
    private:
        static size_t requestHandlerCount;
        Repository * db;
        FuncMap * funcs;

    public:
        RequestHandler();
        ~RequestHandler();

        virtual void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) {
            const size_t method = HTTP::Methods[request.getMethod()];
            funcs[method][request.getURI()](request, response);
        }

        void setRoutes();
};

#endif // REQUEST_HANDLER_H
