#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <vector>
#include <iostream>
#include <Lexer.hpp>
#include <Parser.hpp>

#define DEFAULT_CONFIG_FILE "config/default.conf"

class WebServer
{
private:
    std::vector<ServerInfo> serverInfo;

public:
    void StartServer();

    WebServer();
    WebServer(const char *filePath);
    ~WebServer();
};

#endif