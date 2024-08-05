#ifndef SERVERINFO_HPP
#define SERVERINFO_HPP

#include <string>

class ServerInfo
{
private:
    int         port;
    std::string name;
    std::string host;
    std::string defaultErrorPage;

public:
    ServerInfo();
    ~ServerInfo();
};


#endif