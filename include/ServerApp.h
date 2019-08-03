#ifndef SERVER_APP_H
#define SERVER_APP_H

#include "RequestHandlerFactory.h"

class ServerApp : public ServerApplication
{
    protected:
        void initialize(Application& self) {
            try {
                loadConfiguration("application.properties");
                cout << "### configuration loaded : application.properties" << endl;
            } catch (Poco::FileNotFoundException& e) {
                cout << "### " << e.what() << " : application.properties, try: `make sync` to resolve" << endl;
                cout << "### using default configurations" << endl;
            }
            ServerApplication::initialize(self);
        }

        void uninitialize() {
            ServerApplication::uninitialize();
        }

        int main(const vector<string> & inputs) {
            if (inputs.size() > 0) {
                cout << "### Starting server with inputs: ";
                for (auto const & input : inputs) { cout << input << "; "; }
                cout << endl;
            }

            if (!Repository::existsPool()) {
                string port( config().getString("database.port", "3306") );
                string db(getenv( config().getString("database.name", "DB_DATABASE").c_str() ));
                string user(getenv( config().getString("database.username", "DB_USERNAME").c_str() ));
                string password(getenv( config().getString("database.password", "DB_PASSWORD").c_str() ));

                string host="127.0.0.1", compress="true", autoReconnect="true";
                string connectionString(
                        "host=" + host +
                        ";user=" + user +
                        ";password=" + password +
                        ";db=" + db +
                        ";port=" + port +
                        ";compress=" + compress +
                        ";auto-reconnect=" + autoReconnect + ";");

                size_t minSessions = 1, maxSessions = 32, idleTime = 60;
                Repository::init(Poco::Data::MySQL::Connector::KEY,
                        connectionString, minSessions, maxSessions, idleTime);
            }

            size_t serverPort = config().getInt("server.port", 8080); // default port = 8080
            ServerSocket socket(serverPort);
            HTTPServer server(new RequestHandlerFactory, socket, new HTTPServerParams);

            server.start();
            cout << "### Server started" << endl;

            waitForTerminationRequest();  // wait for CTRL-C or kill

            cout << "### Shutting down..." << endl;
            server.stop();
            Repository::reset();

            return Application::EXIT_OK;
        }
};

#endif
