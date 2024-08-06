#include <WebServer.hpp>

int main(int argc, char **argv)
{
    WebServer *webServer;

    if (argc == 1)
        webServer = new WebServer();
    if (argc == 2) 
        webServer = new WebServer(argv[1]);

    webServer->InitServer();
    webServer->StartServer();

    delete webServer;
}