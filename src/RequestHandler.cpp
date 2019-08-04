#include "RequestHandler.h"

size_t RequestHandler::requestHandlerCount = 0;

RequestHandler::RequestHandler() : db(new Repository), funcs(new FuncMap[HTTP::NUM_HTTP_METHODS]), HTTPRequestHandler()
{
    mtx.lock(); ++requestHandlerCount; mtx.unlock();
    setRoutes();
}

RequestHandler::~RequestHandler() {
    if (db) { delete db; }
    if (funcs) { delete [] funcs; }
}

void RequestHandler::setRoutes() {
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
