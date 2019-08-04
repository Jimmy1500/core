#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Stringifier.h>
#include <Poco/Dynamic/Var.h>

#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Util/ServerApplication.h>

#include "Repository.h"

using namespace Poco::Net;
using namespace Poco::Util;
using namespace Poco::JSON;
using namespace std;

typedef map<string, function<void(HTTPServerRequest& request, HTTPServerResponse& response)>> FnMap;

class RequestHandler : public HTTPRequestHandler {
    private:
        static size_t requestHandlerCount;
        Repository * db;
        FnMap * funcs;

    public:
        RequestHandler() : db(new Repository), funcs(new FnMap[HTTP::NUM_HTTP_METHODS]), HTTPRequestHandler()
        {
            mtx.lock(); ++requestHandlerCount; mtx.unlock();
            funcs[HTTP::GET].emplace
                ("/",
                    [this](HTTPServerRequest& request, HTTPServerResponse& response)->void {
                        response.setStatus(HTTPResponse::HTTP_OK);
                        response.setContentType("application/json");

                        DAO::Tenant tenant; db->popById(1, tenant);
                        cout << tenant.id << ", " << tenant.name << endl;
                        ostream& out = response.send();
                        Poco::JSON::Object::Ptr obj = new Poco::JSON::Object; // smart ptr (auto GC)
                        obj->set("host", request.getHost());
                        obj->set("uri", request.getURI());
                        obj->set("method", request.getMethod());
                        obj->set("count", requestHandlerCount);
                        obj->set("message", "hello world!");
                        obj->stringify(out);
                        out.flush();
                        cout << "Response # " << requestHandlerCount <<
                        " sent for URI=" << request.getURI() << endl;
                    }
                );
        }

        ~RequestHandler() {
            if (db) { delete db; }
            if (funcs) { delete [] funcs; }
        }

        virtual void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) {
            const size_t method = HTTP::Methods[request.getMethod()];
            funcs[method][request.getURI()](request, response);
        }

};

size_t RequestHandler::requestHandlerCount = 0;

#endif // REQUEST_HANDLER_H
