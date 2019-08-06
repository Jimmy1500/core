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

#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>

#include "Repository.h"

#ifndef ROUTE
#define ROUTE(MAP)  \
        MAP.emplace
#endif

#ifndef JSON_WRITE
#define JSON_WRITE(TO, WRAPPER, WRITER)     \
        WRAPPER wrapper(TO);                \
        WRITER<WRAPPER> writer(wrapper);
#endif

#ifndef JSON_FIELD
#define JSON_FIELD(KEY, VALUE, TYPE)   \
        writer.Key(KEY);               \
        writer.TYPE(VALUE);
#endif

#ifndef JSON_OBJ
#define JSON_OBJ(O)             \
        writer.StartObject();   \
        O                       \
        writer.EndObject();
#endif

#ifndef JSON_ARR
#define JSON_ARR(A)             \
        writer.StartArray();    \
        A                       \
        writer.EndArray();
#endif

#ifndef FOREACH
#define FOREACH(X, A)   \
        for (X) { A }
#endif

using namespace std;
using namespace Poco::Net;
using namespace Poco::JSON;
using namespace Poco::Dynamic;
using namespace rapidjson;

typedef map<string, function<void(HTTPServerRequest&, HTTPServerResponse&)>> RestMap;

class Controller : public HTTPRequestHandler {
    private:
        Poco::JSON::Parser parser;
        Repository db;
        RestMap * restful;

    public:
        Controller();
        ~Controller();

        virtual void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) {
            const size_t method(HTTP::Methods[request.getMethod()]);
            auto rest = restful[method][request.getURI()];
            if (rest) {
                rest(request, response);
            } else {
                ostream& os = response.send();
                os << request.getHost() << request.getURI() << " NOT IMPLEMENTED" << endl;
                os.flush();
            }
            mtx.lock();
            cout << "Response sent for URI=" << request.getURI() << endl;
            mtx.unlock();
        }

        // RESTful
        void mapGet(RestMap &);
        void mapPut(RestMap &);
        void mapPost(RestMap &);
        void mapPatch(RestMap &);
        void mapDelete(RestMap &);
};

#endif // REQUEST_HANDLER_H
