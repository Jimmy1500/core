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
                cout << "Starting server with inputs: ";
                for (auto const & input : inputs) { cout << input << "; "; }
                cout << endl;
            }

            size_t port = config().getInt("server.port", 8080); // default = 8080

            ServerSocket socket(port);
            HTTPServer server(new RequestHandlerFactory, socket, new HTTPServerParams);

            server.start();
            cout << "Server started" << endl;

            waitForTerminationRequest();  // wait for CTRL-C or kill

            cout << "Shutting down..." << endl;
            server.stop();

            return Application::EXIT_OK;
        }
};

#endif
