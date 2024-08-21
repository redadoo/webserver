#include <ServerConfig.hpp>

ServerConfig::ServerConfig()
{
    this->serverPort = 0;
    this->isInitAutoindex = false;
    this->clientMaxBody.size = "0";
}
