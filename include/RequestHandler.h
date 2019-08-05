#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <Poco/Exception.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Stringifier.h>
#include <Poco/Dynamic/Var.h>

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include "Repository.h"

using namespace std;
using namespace Poco::Net;
using namespace Poco::JSON;
using namespace Poco::Dynamic;

typedef map<string, function<void(HTTPServerRequest& request, HTTPServerResponse& response)>> FuncMap;

class RequestHandler : public HTTPRequestHandler {
    private:
        static size_t requestHandlerCount;
        Parser parser;
        Repository db;
        FuncMap * route;

    public:
        RequestHandler();
        ~RequestHandler();

        virtual void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) {
            const size_t method = HTTP::Methods[request.getMethod()];
            route[method][request.getURI()](request, response);
            cout << "Response # " << requestHandlerCount <<
                " sent for URI=" << request.getURI() << endl;
        }

        void mapGets(FuncMap &);
        void mapPuts(FuncMap &);
        void mapPosts(FuncMap &);
        void mapPatches(FuncMap &);
        void mapDeletes(FuncMap &);
};

#endif // REQUEST_HANDLER_H
