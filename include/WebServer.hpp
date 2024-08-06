#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

# include <vector>
# include <iostream>
# include <Lexer.hpp>
# include <Parser.hpp>
# include <sys/socket.h>
# include <netinet/in.h>
# include <sys/select.h>
# include <arpa/inet.h>

#define DEFAULT_CONFIG_FILE "config/default.conf"

class WebServer
{
private:
    std::vector<ServerInfo> serverInfo;

public:
    void StartServer();
    void InitServer();

    WebServer();
    WebServer(const char *filePath);
    ~WebServer();
};

#endif