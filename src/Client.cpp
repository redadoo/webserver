#include <Client.hpp>

Client::Client() : clientFd(-1) {}

Client::Client(int fd, const std::string& ip, uint16_t port)  : clientFd(fd)
{
    this->clientConfig.srcIp = ip;
    this->clientConfig.srcPort = port;
    this->contentLenght = -1;
}

void Client::InitClientValue(const char *response)
{
	(void)response;
}
