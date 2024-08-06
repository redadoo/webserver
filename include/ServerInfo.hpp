#ifndef SERVERINFO_HPP
#define SERVERINFO_HPP

#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>

#define MAX_EVENTS 10
#define MAX_CLIENTS 10

class ErrorOnSocketCreation : public std::exception {
	public:
		virtual const char* what() const throw() { return "error when try to create socket";}
};

class ErrorOnBindSocket : public std::exception {
	public:
		virtual const char* what() const throw() { return "error when try to bind socket";}
};

class ErrorOnListenSocket : public std::exception {
	public:
		virtual const char* what() const throw() { return "error when try to listen socket";}
};

class ErrorOnEpollCreation : public std::exception {
	public:
		virtual const char* what() const throw() { return "error when try to create epoll instance";}
};

struct ServerInfo
{
	int                 serverFd; 
	int                 epollFd;
	struct sockaddr_in  serverAddress;
	struct epoll_event  event; 
	struct epoll_event  events[MAX_EVENTS];

	const int           port;
	const int           clientMaxBodySize;
	const std::string   index;
	const std::string   name;
	const std::string   root;
	const std::string   host;
	const std::string   defaultErrorPage;

	ServerInfo(int _port, int _clientMaxBodySize,std::string _index, 
				std::string _name, std::string _root, std::string _host, std::string _defaultErrorPage);

    ~ServerInfo();
	
    void InitInfo();

};


#endif