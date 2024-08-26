#include <Client.hpp>

//constructor/destructor

Client::Client() : clientFd(-1) {}

Client::Client(int fd, const std::string& ip, uint16_t port)  : clientFd(fd)
{
    this->clientConfig.srcIp = ip;
    this->clientConfig.srcPort = port;
    this->contentLenght = -1;
}