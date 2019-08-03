#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

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

class RequestHandler : public HTTPRequestHandler {
    private:
        static size_t count;

    public:
        RequestHandler() : HTTPRequestHandler() {
            mtx.lock(); ++count; mtx.unlock();
        }

        virtual void handleRequest(HTTPServerRequest &request, HTTPServerResponse &response) {
            const string & method = request.getMethod();
            switch (HTTP::Methods[method]) {
                case HTTP::GET:
                    handleGet(request, response);
                    break;
                case HTTP::POST:
                    handlePost(request, response);
                    break;
                case HTTP::PUT:
                    handlePut(request, response);
                    break;
                case HTTP::PATCH:
                    handlePatch(request, response);
                    break;
                case HTTP::DELETE:
                    handleDelete(request, response);
                    break;
                default:
                    break;

            }
        }

        virtual void handleGet(HTTPServerRequest &request, HTTPServerResponse &response) {
            response.setStatus(HTTPResponse::HTTP_OK);
            response.setContentType("application/json");

            ostream& out = response.send();
            Poco::JSON::Object::Ptr obj = new Poco::JSON::Object; // smart ptr (auto GC)
            obj->set("host", request.getHost());
            obj->set("uri", request.getURI());
            obj->set("method", request.getMethod());
            obj->set("count", count);
            obj->set("message", "hello world!");
            obj->stringify(out);
            out.flush();

            cout << "Response # " << count << " sent for URI=" << request.getURI() << endl;
        }

        virtual void handlePost(HTTPServerRequest &request, HTTPServerResponse &response) {
            response.setStatus(HTTPResponse::HTTP_OK);
            ostream& out = response.send();
            out << "Post: Not Implemented";
            out.flush();
            cout << "Response # " << count << " sent for URI=" << request.getURI() << endl;
        }

        virtual void handlePut(HTTPServerRequest &request, HTTPServerResponse &response) {
            response.setStatus(HTTPResponse::HTTP_OK);
            ostream& out = response.send();
            out << "Put: Not Implemented";
            out.flush();
            cout << "Response # " << count << " sent for URI=" << request.getURI() << endl;
        }

        virtual void handlePatch(HTTPServerRequest &request, HTTPServerResponse &response) {
            response.setStatus(HTTPResponse::HTTP_OK);
            ostream& out = response.send();
            out << "PATCH: Not Implemented";
            out.flush();
            cout << "Response # " << count << " sent for URI=" << request.getURI() << endl;
        }

        virtual void handleDelete(HTTPServerRequest &request, HTTPServerResponse &response) {
            response.setStatus(HTTPResponse::HTTP_OK);
            ostream& out = response.send();
            out << "Delete: Not Implemented";
            out.flush();
            cout << "Response # " << count << " sent for URI=" << request.getURI() << endl;
        }
};

size_t RequestHandler::count = 0;

#endif // REQUEST_HANDLER_H
