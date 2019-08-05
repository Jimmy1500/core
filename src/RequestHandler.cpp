#include "RequestHandler.h"

size_t RequestHandler::requestHandlerCount = 0;

RequestHandler::RequestHandler() :
    parser(),
    db(Repository()),
    route(new FuncMap[HTTP::NUM_HTTP_METHODS]),
    HTTPRequestHandler()
{
    mtx.lock(); ++requestHandlerCount; mtx.unlock();
    mapGets(route[HTTP::GET]);
    mapPuts(route[HTTP::PUT]);
    mapPosts(route[HTTP::POST]);
    mapPatches(route[HTTP::PATCH]);
    mapDeletes(route[HTTP::DELETE]);
}

RequestHandler::~RequestHandler() {
    if (route) { delete [] route; }
}

void RequestHandler::mapGets(FuncMap & gets) {
    gets.emplace
        ("/",
         [&](HTTPServerRequest& request, HTTPServerResponse& response)-> void {
         response.setStatus(HTTPResponse::HTTP_OK);
         response.setContentType("application/json");

         Object::Ptr ret = new Object; // smart ptr (auto GC)
         ret->set("host", request.getHost());
         ret->set("uri", request.getURI());
         ret->set("method", request.getMethod());
         ret->set("count", requestHandlerCount);
         ret->set("response", "hello world!");
         ostream& os = response.send();
         ret->stringify(os);
         os.flush();
         }
        );

    gets.emplace
        ("/tenant",
         [&](HTTPServerRequest& request, HTTPServerResponse& response)-> void {
         response.setStatus(HTTPResponse::HTTP_OK);
         response.setContentType("application/json");

         Object::Ptr ret = new Object;
         Object::Ptr req = parser.parse(request.stream()).extract<Object::Ptr>();
         ret->set("request", req);

         int tenant_id = req->getValue<int>("tenant_id");
         DAO::Tenant tenant;
         db.popById(tenant_id, tenant);
         Object::Ptr resp = new Object;
         resp->set("tenant_id", tenant.id);
         resp->set("tenant_name", tenant.name);
         ret->set("response", resp);

         ostream& os = response.send();
         ret->stringify(os);
         os.flush();
         }
        );
}

void RequestHandler::mapPuts(FuncMap & puts) { }
void RequestHandler::mapPosts(FuncMap & posts) { }
void RequestHandler::mapPatches(FuncMap & patches) { }
void RequestHandler::mapDeletes(FuncMap & deletes) { }
