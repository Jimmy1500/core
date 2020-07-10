#ifndef SERVER_APP_H
#define SERVER_APP_H

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Util/ServerApplication.h>

#include "RequestHandlerFactory.h"

using namespace std;
using namespace Poco::Util;

class ServerApp : public ServerApplication
{
    protected:
        void initialize(Application& self) {
            try {
                loadConfiguration("application.properties");
                cout << "### configuration loaded : application.properties" << endl;

                if (!Repository::existsPool()) {
                    string connector( config().getString("database.type", Poco::Data::MySQL::Connector::KEY) );
                    string port( config().getString("database.port", "3306") );

                    string dbname(getenv( config().getString("database.name", "DB_DATABASE").c_str() ));
                    string user(getenv( config().getString("database.username", "DB_USERNAME").c_str() ));
                    string password(getenv( config().getString("database.password", "DB_PASSWORD").c_str() ));

                    stringstream ss;
                    ss << "host=127.0.0.1;port=" << port << ";user=" << user << ";password=" << password << ";db=" << dbname << ";compress=true;auto-reconnect=true;";
                    string connection = ss.str();
                    size_t minSessions = 1, maxSessions = 32, idleTime = 60;

                    Repository::initialize(
                            connector,
                            connection,
                            minSessions,
                            maxSessions,
                            idleTime);
                    cout << "### Database loaded: [type=MySQL, credential=" << connection << "]" << endl;
                }
            } catch (Poco::FileNotFoundException& e) {
                cout << "### " << e.what() << " : application.properties, try: `make sync` in build directory to resolve" << endl;
                cout << "### using default configurations" << endl;
            } catch (Poco::Data::DataException& e) {
                cout << "### " << e.what() << endl;
            } catch (Poco::Exception& e) {
                cout << "### " << e.what() << endl;
            } catch (std::exception& e) {
                cout << "### " << e.what() << endl;
            }
            ServerApplication::initialize(self);
        }

        void uninitialize() {
            Repository::uninitialize();
            ServerApplication::uninitialize();
        }

        int main(const vector<string> & inputs) {
            try {
                if (inputs.size() > 0) {
                    cout << "### Starting server with inputs: ";
                    for (auto const & input : inputs) { cout << input << "; "; }
                    cout << endl;
                }

                ServerSocket socket(config().getInt("server.port", 8080)); // default port = 8080
                HTTPServer server(new RequestHandlerFactory, socket, new HTTPServerParams);

                server.start();
                cout << "### Server started" << endl;

                waitForTerminationRequest();  // wait for CTRL-C or kill

                cout << "### Server shutting down..." << endl;
                server.stop();
            } catch (Poco::Data::DataException& e) {
                cout << "### " << e.what() << endl;
                return Application::EXIT_DATAERR;
            } catch (Poco::Exception& e) {
                cout << "### " << e.what() << endl;
                return Application::EXIT_SOFTWARE;
            } catch (std::exception& e) {
                cout << "### " << e.what() << endl;
                return Application::EXIT_UNAVAILABLE;
            }
            

            return Application::EXIT_OK;
        }
};

#endif
