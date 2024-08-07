#include <ServerInfo.hpp>

using namespace WebServerExceptions;

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

    // Create socket
    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd == -1) {
        throw ErrorOnSocketCreation();
    }

    Logger::Log("created socket successfully");

    // Bind socket to address and port
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);
    if (bind(serverFd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        close(serverFd);
        throw ErrorOnBindSocket();
    }

    Logger::Log("Bind socket to address successfully");

    // Listen for incoming connections
    if (listen(serverFd, MAX_CLIENTS) == -1) {
        close(serverFd);
        throw ErrorOnListenSocket();
    }

    Logger::Log(std::string("Socket Listen for incoming connections on port : ") + utils::IntToString(port));

    // Create epoll instance
    epollFd = epoll_create1(0);
    if (epollFd == -1) {
        close(serverFd);
        throw ErrorOnEpollCreation();
    }
    
    Logger::Log("Create epoll instance successfully");

    // Add server socket to epoll
    event.events = EPOLLIN;
    event.data.fd = serverFd;
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverFd, &event) == -1) {
        close(serverFd);
        close(epollFd);
        throw ErrorOnAddSocketToEpoll();
    }

    Logger::Log("Add server socket to epoll successfully");

}

ServerInfo::~ServerInfo() {}
