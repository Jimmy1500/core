#include "Controller.h"

Controller::Controller() :
    parser(),
    db(),
    restful(new RestMap[HTTP::NUM_HTTP_METHODS]),
    HTTPRequestHandler()
{
    mtx.lock(); ++SYS::registry.controllerCount; mtx.unlock();
    wire();
}

Controller::~Controller() {
    if (restful) { delete [] restful; }
    mtx.lock(); --SYS::registry.controllerCount; mtx.unlock();
}

void Controller::wire() {
    ROUTE(HTTP::GET, "/",
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

    ROUTE(HTTP::GET, "/tenant",
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

    ROUTE(HTTP::GET, "/tenants",
        [&](HTTPServerRequest& request, HTTPServerResponse& response)-> void {
            response.setStatus(HTTPResponse::HTTP_OK);
            response.setContentType("application/json");

            ostream& os = response.send();
            // JSON_WRITE(os, rapidjson::OStreamWrapper, rapidjson::PrettyWriter)
            JSON_WRITE(os, rapidjson::OStreamWrapper, rapidjson::Writer)
            try {
                vector<DAO::Tenant> tenants;
                db.popAll(tenants);

                JSON_ARR(
                    FOREACH(const auto& tenant : tenants,
                        JSON_OBJ(
                            JSON_FIELD("tenant_id", tenant.id, Uint);
                            JSON_FIELD("tenant_name", tenant.name.c_str(), String);
                        )
                    )
                )
            } catch (Poco::Data::MySQL::ConnectionException& e) {
                JSON_OBJ( JSON_FIELD("response", e.what(), String) )
            } catch (Poco::Data::MySQL::StatementException& e) {
                JSON_OBJ( JSON_FIELD("response", e.what(), String) )
            } catch (Poco::JSON::JSONException& e) {
                JSON_OBJ( JSON_FIELD("response", e.what(), String) )
            } catch (std::exception& e) {
                JSON_OBJ( JSON_FIELD("response", e.what(), String) )
            }
            os.flush();
        }
    );

}
