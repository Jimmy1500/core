#include "Controller.h"

Controller::Controller() :
    parser(),
    database(),
    restMap(new RestMap[HTTP::NUM_HTTP_METHODS]),
    HTTPRequestHandler()
{
    mtx.lock(); ++SYS::registry.controllerCount; mtx.unlock();
    wireRoutes();
}

Controller::~Controller() {
    if (restMap) { delete [] restMap; }
    mtx.lock(); --SYS::registry.controllerCount; mtx.unlock();
}

void Controller::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) {
    const size_t method(HTTP::Methods[request.getMethod()]);
    auto f = restMap[method][request.getURI()];
    if (f) {
        f(request, response);
    } else {
        ostream& os = response.send();
        os << request.getHost() << request.getURI() << " NOT IMPLEMENTED" << endl;
        os.flush();
    }
    mtx.lock();
    cout << "Response sent for URI=" << request.getURI() << endl;
    mtx.unlock();
}

void Controller::wireRoutes() {
    ROUTE(HTTP::GET, "/",
        [&](HTTPServerRequest& request, HTTPServerResponse& response)-> void {
            response.setStatus(HTTPResponse::HTTP_OK);
            response.setContentType("application/json");
            Object::Ptr ret = new Object;
            try {
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
            response.setStatus(HTTPResponse::HTTP_OK);
            response.setContentType("application/json");
            Object::Ptr ret = new Object;
            try {
                Object::Ptr req = parser.parse(request.stream()).extract<Object::Ptr>();
                ret->set("request", req);

                int tenantId = req->getValue<int>("tenant_id");
                DAO::Tenant tenant; database.popById(tenantId, tenant);

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
            } catch (Poco::Exception& e) {
                ret->set("response", e.what());
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
                database.popAll(tenants);

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
            } catch (Poco::Exception& e) {
                JSON_OBJ( JSON_FIELD("response", e.what(), String) )
            } catch (std::exception& e) {
                JSON_OBJ( JSON_FIELD("response", e.what(), String) )
            }
            os.flush();
        }
    );

}
