#include <Client.hpp>

//constructor/destructor

Client::Client() {}

Client::Client(int fd, const std::string& _ip, uint16_t _port)  : clientFd(fd)
{
    this->request.startLine = StartLine();
    this->request.body.Init();
    this->request.isHeaderComplete = false;
    this->clientConfig.ip = _ip;
    this->clientConfig.port.port = _port;
    this->contentLenght = -1;
}