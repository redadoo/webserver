#include <Client.hpp>

//constructor/destructor

Client::Client() : clientFd(-1) {}

Client::Client(int fd, const std::string& _ip, uint16_t _port)  : clientFd(fd)
{
    this->clientConfig.ip = _ip;
    this->clientConfig.port.port = _port;
    this->contentLenght = -1;
}