#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Util/ServerApplication.h>
#include <iostream>
#include <string>
#include <vector>

using namespace Poco::Net;
using namespace Poco::Util;
using namespace std;

class MyRequestHandler : public HTTPRequestHandler
{
    public:
        virtual void handleRequest(HTTPServerRequest &req, HTTPServerResponse &resp)
        {
            resp.setStatus(HTTPResponse::HTTP_OK);
            resp.setContentType("text/html");

            ostream& out = resp.send();
            // out << "<h1>Hello world!</h1>"
            //     << "<p>Request Count: " << ++count  << "</p>"
            //     << "<p>Host: "   << req.getHost()   << "</p>"
            //     << "<p>Method: " << req.getMethod() << "</p>"
            //     << "<p>URI: "    << req.getURI()    << "</p>";

            out << "<!DOCTYPE html>"
                << "<html>"
                << "<head>"
                << "	<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">"
                << "	<title>3 Column Layout</title>"
                << "	<style type=\"text/css\">"
                << "		body {"
                << "			min-width: 630px;"
                << "		}"
                << "		#container {"
                << "			padding-left: 200px;"
                << "			padding-right: 190px;"
                << "		}"
                << "		#container .column {"
                << "			position: relative;"
                << "			float: left;"
                << "		}"
                << "		#center {"
                << "			padding: 10px 20px;"
                << "			width: 100%;"
                << "		}"
                << "		#left {"
                << "			width: 180px;"
                << "			padding: 0 10px;"
                << "			right: 240px;"
                << "			margin-left: -100%;"
                << "		}"
                << "		#right {"
                << "			width: 130px;"
                << "			padding: 0 10px;"
                << "			margin-right: -100%;"
                << "		}"
                << "		#footer {"
                << "			clear: both;"
                << "		}"
                << "		* html #left {"
                << "			left: 150px;"
                << "		}"
                << "		#container {"
                << "			overflow: hidden;"
                << "		}"
                << "		#container .column {"
                << "			padding-bottom: 1001em;"
                << "			margin-bottom: -1000em;"
                << "		}"
                << "		* html body {"
                << "			overflow: hidden;"
                << "		}"
                << "		* html #footer-wrapper {"
                << "			float: left;"
                << "			position: relative;"
                << "			width: 100%;"
                << "			padding-bottom: 10010px;"
                << "			margin-bottom: -10000px;"
                << "			background: #fff;"
                << "		}"
                << "		body {"
                << "			margin: 0;"
                << "			padding: 0;"
                << "			font-family:Sans-serif;"
                << "			line-height: 1.5em;"
                << "		}"
                << "		p {"
                << "			color: #555;"
                << "		}"
                << "		nav ul {"
                << "			list-style-type: none;"
                << "			margin: 0;"
                << "			padding: 0;"
                << "		}"
                << "		nav ul a {"
                << "			color: darkgreen;"
                << "			text-decoration: none;"
                << "		}"
                << "		#header, #footer {"
                << "			font-size: large;"
                << "			padding: 0.3em;"
                << "			background: #BCCE98;"
                << "		}"
                << "		#left {"
                << "			background: #DAE9BC;"
                << "		}"
                << "		#right {"
                << "			background: #F7FDEB;"
                << "		}"
                << "		#center {"
                << "			background: #fff;"
                << "		}"
                << "		#container .column {"
                << "			padding-top: 1em;"
                << "		}"
                << "	</style>"
                << "	<script type=\"text/javascript\">"
                << "		/* ============================="
                << "		This script generates sample text for the body content. "
                << "		You can remove this script and any reference to it. "
                << "		 ============================= */"
                << "		var bodyText=[\"The smaller your reality, the more convinced you are that you know everything.\", \"If the facts don't fit the theory, change the facts.\", \"The past has no power over the present moment.\", \"This, too, will pass.\", \"</p><p>You will not be punished for your anger, you will be punished by your anger.\", \"Peace comes from within. Do not seek it without.\", \"<h3>Heading</h3><p>The most important moment of your life is now. The most important person in your life is the one you are with now, and the most imp\"]"
                << "		function generateText(sentenceCount){"
                << "			for (var i=0; i<sentenceCount; i++)"
                << "			document.write(bodyText[Math.floor(Math.random()*7)]+\" \")"
                << "		}"
                << "	</script>"
                << "</head>"
                << "<body>"
                << "	<header id=\"header\"><p>Hello world!</p></header>"
                << "	<div id=\"container\">"
                << "		<main id=\"center\" class=\"column\">"
                << "			<article>"
                << "				<h1>Method: HTTP " << req.getMethod() << " : " << req.getHost() << "</h1>"
                << "				<p><script>generateText(50)</script></p>"
                << "			</article>"
                << "		</main>"
                << "		<nav id=\"left\" class=\"column\">"
                << "			<h3>Request benchmark</h3>"
                << "			<ul>"
                << "				<li><a href=\"#\">Link 1</a></li>"
                << "				<li><a href=\"#\">Link 2</a></li>"
                << "				<li><a href=\"#\">Link 3</a></li>"
                << "				<li><a href=\"#\">Link 4</a></li>"
                << "				<li><a href=\"#\">Link 5</a></li>"
                << "			</ul>"
                << "			<h3>Query benchmark</h3>"
                << "			<ul>"
                << "				<li><a href=\"#\">Link 1</a></li>"
                << "				<li><a href=\"#\">Link 2</a></li>"
                << "				<li><a href=\"#\">Link 3</a></li>"
                << "				<li><a href=\"#\">Link 4</a></li>"
                << "				<li><a href=\"#\">Link 5</a></li>"
                << "			</ul>"
                << "		</nav>"
                << "		<div id=\"right\" class=\"column\">"
                << "			<h3>URI : " << req.getURI() << "</h3>"
                << "			<p><script>generateText(1)</script></p>"
                << "		</div>"
                << "	</div>"
                << "	<div id=\"footer-wrapper\">"
                << "		<footer id=\"footer\"><p>Request Count : " << ++count << "</p></footer>"
                << "	</div>"
                << "</body>"
                << "</html>";
            out.flush();

            cout << endl
                << "Response sent for count=" << count
                << " and URI=" << req.getURI() << endl;
        }

    private:
        static int count;
};

int MyRequestHandler::count = 0;

class MyRequestHandlerFactory : public HTTPRequestHandlerFactory
{
    public:
        virtual HTTPRequestHandler* createRequestHandler(const HTTPServerRequest &)
        {
            return new MyRequestHandler;
        }
};

class MyServerApp : public ServerApplication
{
    protected:
        int main(const vector<string> & xs)
        {
            if (xs.size() > 0 ) {
                cout << "Firing up server with inputs: ";
                for (auto const & x : xs) {
                    cout << x << "; ";
                }
                cout << endl;
            }

            HTTPServer s(new MyRequestHandlerFactory, ServerSocket(8080), new HTTPServerParams);

            s.start();
            cout << endl << "Server started" << endl;

            waitForTerminationRequest();  // wait for CTRL-C or kill

            cout << endl << "Shutting down..." << endl;
            s.stop();

            return Application::EXIT_OK;
        }
};
