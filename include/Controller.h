#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <Poco/Exception.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Stringifier.h>
#include <Poco/Dynamic/Var.h>

#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>

#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>

#include "Repository.h"

#ifndef ROUTE
#define ROUTE(METHOD, ENDPOINT, FUNCTOR)  \
        restMap[METHOD].emplace(ENDPOINT, FUNCTOR)
#endif

#ifndef JSON_WRITE
#define JSON_WRITE(TARGET, WRAPPER, WRITER)             \
        WRAPPER rapidjson_wrapper(TARGET);              \
        WRITER<WRAPPER> rapidjson_writer(rapidjson_wrapper);
#endif

#ifndef JSON_FIELD
#define JSON_FIELD(KEY, VALUE, TYPE)    \
        rapidjson_writer.Key(KEY);      \
        rapidjson_writer.TYPE(VALUE);
#endif

#ifndef JSON_OBJ
#define JSON_OBJ(O)                         \
        rapidjson_writer.StartObject();     \
        O                                   \
        rapidjson_writer.EndObject();
#endif

#ifndef JSON_ARR
#define JSON_ARR(A)                         \
        rapidjson_writer.StartArray();      \
        A                                   \
        rapidjson_writer.EndArray();
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
        Repository database;
    protected:
        RestMap * restMap;
    public:
        Controller();
        ~Controller();

        virtual void handleRequest(HTTPServerRequest &, HTTPServerResponse &);
        virtual void wireRoutes(); // RESTful
};

#endif // REQUEST_HANDLER_H
