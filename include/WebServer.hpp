#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <ServerInfo.hpp>

class WebServer
{
private:
    ServerInfo serverInfo;

public:
    void StartServer();

    WebServer();
    WebServer(const char *filePath);
    ~WebServer();
};

#endif