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
#define JSON_WRITE(TARGET, WRAPPER, WRITER)         \
        WRAPPER TARGET_wrapper(TARGET);             \
        WRITER<WRAPPER> writer(TARGET_wrapper);
#endif

#ifndef JSON_FIELD
#define JSON_FIELD(KEY, VALUE, TYPE)    \
        writer.Key(KEY);                \
        writer.TYPE(VALUE);
#endif

#ifndef JSON_OBJ
#define JSON_OBJ(O)               \
        writer.StartObject();     \
        O                         \
        writer.EndObject();
#endif

#ifndef JSON_ARR
#define JSON_ARR(A)               \
        writer.StartArray();      \
        A                         \
        writer.EndArray();
#endif

using namespace std;
using namespace Poco::Net;
using namespace Poco::Dynamic;
using namespace Poco::JSON;
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
