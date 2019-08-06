#include "Controller.h"

Controller::Controller(size_t id) :
    id(id),
    parser(),
    db(),
    funcs(new FuncMap[HTTP::NUM_HTTP_METHODS]),
    HTTPRequestHandler()
{
    mapGet(funcs[HTTP::GET]);
    mapPut(funcs[HTTP::PUT]);
    mapPost(funcs[HTTP::POST]);
    mapPatch(funcs[HTTP::PATCH]);
    mapDelete(funcs[HTTP::DELETE]);
}

Controller::~Controller() {
    if (funcs) { delete [] funcs; }
    mtx.lock(); --REGISTRY::handlerCount; mtx.unlock();
}

void Controller::mapGet(FuncMap & gets) {
    gets.emplace ("/",
            [&](HTTPServerRequest& request, HTTPServerResponse& response)-> void {
            Object::Ptr ret = new Object;
            try {
            response.setStatus(HTTPResponse::HTTP_OK);
            response.setContentType("application/json");
            ret->set("host", request.getHost());
            ret->set("uri", request.getURI());
            ret->set("method", request.getMethod());
            ret->set("request_id", id);
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

    gets.emplace ("/tenant",
            [&](HTTPServerRequest& request, HTTPServerResponse& response)-> void {
            Object::Ptr ret = new Object;
            try {
            response.setStatus(HTTPResponse::HTTP_OK);
            response.setContentType("application/json");

            Object::Ptr req = parser.parse(request.stream()).extract<Object::Ptr>();
            ret->set("request", req);

            int tenantId = req->getValue<int>("tenant_id");
            DAO::Tenant tenant; db.popById(tenantId, tenant);

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

    gets.emplace ("/tenants",
            [&](HTTPServerRequest& request, HTTPServerResponse& response)-> void {
            ostream& os = response.send();
            OStreamWrapper osw(os);

            //PrettyWriter<OStreamWrapper> writer(osw);
            Writer<OStreamWrapper> writer(osw);

            try {
            response.setStatus(HTTPResponse::HTTP_OK);
            response.setContentType("application/json");

            vector<DAO::Tenant> tenants; db.popAll(tenants);

            writer.StartArray();
            for (DAO::Tenant const & tenant : tenants) {
                writer.StartObject();
                writer.Key("tenant_id"); writer.Uint(tenant.id);
                writer.Key("tenant_name"); writer.String(tenant.name.c_str());
                writer.EndObject();
            }
            writer.EndArray();
            os.flush();
            } catch (Poco::Data::MySQL::ConnectionException& e) {
                writer.StartObject();
                writer.Key("response"); writer.String(e.what());
                writer.EndObject();
            } catch (Poco::Data::MySQL::StatementException& e) {
                writer.StartObject();
                writer.Key("response"); writer.String(e.what());
                writer.EndObject();
            } catch (Poco::JSON::JSONException& e) {
                writer.StartObject();
                writer.Key("response"); writer.String(e.what());
                writer.EndObject();
            } catch (std::exception& e) {
                writer.StartObject();
                writer.Key("response"); writer.String(e.what());
                writer.EndObject();
            }
            }
    );

}

void Controller::mapPut(FuncMap & puts) { }
void Controller::mapPost(FuncMap & posts) { }
void Controller::mapPatch(FuncMap & patches) { }
void Controller::mapDelete(FuncMap & deletes) { }
