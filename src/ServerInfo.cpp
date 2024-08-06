#include <ServerInfo.hpp>

ServerInfo::ServerInfo(
    int _port, 
    int _clientMaxBodySize, 
    std::string _index,
    std::string _name, 
    std::string _root, 
    std::string _host, 
    std::string _defaultErrorPage
) : 
    port(_port), 
    clientMaxBodySize(_clientMaxBodySize), 
    index(_index),
    name(_name), 
    root(_root), 
    host(_host), 
    defaultErrorPage(_defaultErrorPage) {}


void ServerInfo::InitInfo() 
{
    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd == -1) {
        throw ErrorOnSocketCreation();
        return;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);
    
    if (bind(serverFd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        close(serverFd);
        throw ErrorOnBindSocket();
    }

    // Listen for incoming connections
    if (listen(serverFd, MAX_CLIENTS) == -1) {
        close(serverFd);
        throw ErrorOnListenSocket();
    }

    // Create epoll instance
    epollFd = epoll_create1(0);
    if (epollFd == -1) {
        close(serverFd);
        throw ErrorOnEpollCreation();
    }

    close(serverFd);
}

ServerInfo::~ServerInfo() {}
