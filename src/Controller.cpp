#include "Controller.h"

size_t Controller::requestHandlerCount = 0;

Controller::Controller() :
    parser(),
    db(),
    funcs(new FuncMap[HTTP::NUM_HTTP_METHODS]),
    HTTPRequestHandler()
{
    mtx.lock(); ++requestHandlerCount; mtx.unlock();
    mapGet(funcs[HTTP::GET]);
    mapPut(funcs[HTTP::PUT]);
    mapPost(funcs[HTTP::POST]);
    mapPatch(funcs[HTTP::PATCH]);
    mapDelete(funcs[HTTP::DELETE]);
}

Controller::~Controller() {
    if (funcs) { delete [] funcs; }
}

void Controller::mapGet(FuncMap & gets) {
    gets.emplace
        ("/",
         [&](HTTPServerRequest& request, HTTPServerResponse& response)-> void {
            Object::Ptr ret = new Object;
            try {
               response.setStatus(HTTPResponse::HTTP_OK);
               response.setContentType("application/json");

               ret->set("host", request.getHost());
               ret->set("uri", request.getURI());
               ret->set("method", request.getMethod());
               ret->set("count", requestHandlerCount);
               ret->set("response", "hello world!");

               Object::Ptr req = parser.parse(request.stream()).extract<Object::Ptr>();
               ret->set("request", req);
            } catch (Poco::JSON::JSONException& e) {
                ret->set("response", e.what());
            } catch (std::exception& e) {
                ret->set("response", e.what());
            }

            ostream& os = response.send();
            ret->stringify(os);
            os.flush();
         }
        );

    gets.emplace
        ("/tenant",
         [&](HTTPServerRequest& request, HTTPServerResponse& response)-> void {
            Object::Ptr ret = new Object;
            try {
               response.setStatus(HTTPResponse::HTTP_OK);
               response.setContentType("application/json");

               Object::Ptr req = parser.parse(request.stream()).extract<Object::Ptr>();
               ret->set("request", req);
               int tenant_id = req->getValue<int>("tenant_id");
               DAO::Tenant tenant;
               db.popById(tenant_id, tenant);
               
               Object::Ptr resp = new Object;
               resp->set("tenant_id", tenant.id);
               resp->set("tenant_name", tenant.name);
               ret->set("response", resp);
            } catch (Poco::Data::MySQL::ConnectionException& e) {
                ret->set("response", e.what());
            } catch (Poco::Data::MySQL::StatementException& e) {
                ret->set("response", e.what());
            } catch (Poco::JSON::JSONException& e) {
                ret->set("response", e.what());
            } catch (std::exception& e) {
                ret->set("response", e.what());
            }

            ostream& os = response.send();
            ret->stringify(os);
            os.flush();
         }
      );
}

void Controller::mapPut(FuncMap & puts) { }
void Controller::mapPost(FuncMap & posts) { }
void Controller::mapPatch(FuncMap & patches) { }
void Controller::mapDelete(FuncMap & deletes) { }
