#include "Controller.h"

Controller::Controller() :
    parser(),
    db(),
    restful(new RestMap[HTTP::NUM_HTTP_METHODS]),
    HTTPRequestHandler()
{
    mtx.lock(); ++SYS::registry.controllerCount; mtx.unlock();
    mapGet(restful[HTTP::GET]);
    mapPut(restful[HTTP::PUT]);
    mapPost(restful[HTTP::POST]);
    mapPatch(restful[HTTP::PATCH]);
    mapDelete(restful[HTTP::DELETE]);
}

Controller::~Controller() {
    if (restful) { delete [] restful; }
    mtx.lock(); --SYS::registry.controllerCount; mtx.unlock();
}

void Controller::mapGet(RestMap & http_get) {
    ROUTE(http_get)
        ("/",
        [&](HTTPServerRequest& request, HTTPServerResponse& response)-> void {
            Object::Ptr ret = new Object;
            try {
                response.setStatus(HTTPResponse::HTTP_OK);
                response.setContentType("application/json");
                ret->set("host", request.getHost());
                ret->set("uri", request.getURI());
                ret->set("method", request.getMethod());
                ret->set("response", "hello world!");

                Object::Ptr req = parser.parse(request.stream()).extract<Object::Ptr>();
                ret->set("request", req);
            } catch (Poco::JSON::JSONException& e) {
                ret->set("request", e.what());
            } catch (std::exception& e) {
                ret->set("response", e.what());
            }

            ostream& os = response.send();
            ret->stringify(os);
            os.flush();
        }
    );

    ROUTE(http_get)
        ("/tenant",
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
                ret->set("request", e.what());
            } catch (std::exception& e) {
                ret->set("response", e.what());
            }

            ostream& os = response.send();
            ret->stringify(os);
            os.flush();
        }
    );

    ROUTE(http_get)
        ("/tenants",
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
            os.flush();
        }
    );

}

void Controller::mapPut(RestMap & http_put) { }
void Controller::mapPost(RestMap & http_post) { }
void Controller::mapPatch(RestMap & http_patch) { }
void Controller::mapDelete(RestMap & http_delete) { }
