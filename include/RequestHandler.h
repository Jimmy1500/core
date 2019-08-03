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

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>

using namespace Poco::Net;
using namespace Poco::Util;
using namespace Poco::JSON;
using namespace std;

static mutex mtx;
class RequestHandler : public HTTPRequestHandler {
    private:
        static size_t count;

    public:
        RequestHandler() : HTTPRequestHandler() {
            mtx.lock(); ++count; mtx.unlock();
        }

        virtual void handleRequest(HTTPServerRequest &request, HTTPServerResponse &response) {
            if (request.getMethod() == "GET") {
                handleGet(request, response);
            } else if (request.getMethod() == "POST") {
            } else if (request.getMethod() == "PUT") {
            } else if (request.getMethod() == "DELETE") {
            } else if (request.getMethod() == "PATCH") {
            }
        }

        virtual void handleGet(HTTPServerRequest &request, HTTPServerResponse &response) {
            response.setStatus(HTTPResponse::HTTP_OK);
            response.setContentType("application/json");

            ostream& out = response.send();
            // smart pointer, so don't worry about cleaning up
            Poco::JSON::Object::Ptr obj = new Poco::JSON::Object;
            obj->set("host", request.getHost());
            obj->set("uri", request.getURI());
            obj->set("method", request.getMethod());
            obj->set("count", count);
            obj->set("message", "hello world!");
            obj->stringify(out);
            out.flush();

            cout << "Response # " << count << " sent for URI=" << request.getURI() << endl;
        }
};

size_t RequestHandler::count = 0;

class RequestHandlerFactory : public HTTPRequestHandlerFactory
{
    public:
        virtual HTTPRequestHandler* createRequestHandler(const HTTPServerRequest &)
        {
            return new RequestHandler;
        }
};

class ServerApp : public ServerApplication
{
    protected:
        int main(const vector<string> & inputs)
        {
            if (inputs.size() > 0) {
                cout << "Firing up server with inputs: ";
                for (auto const & input : inputs) { cout << input << "; "; }
                cout << endl;
            }

            ServerSocket socket(8080);
            HTTPServer server(new RequestHandlerFactory, socket, new HTTPServerParams);

            s.start();
            cout << "Server started" << endl;

            waitForTerminationRequest();  // wait for CTRL-C or kill

            cout << "Shutting down..." << endl;
            s.stop();

            return Application::EXIT_OK;
        }
};
