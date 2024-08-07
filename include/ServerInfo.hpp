#ifndef SERVERINFO_HPP
#define SERVERINFO_HPP

# include <string>
# include <iostream>
# include <unistd.h>
# include <sys/epoll.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <vector>
# include <WebServerException.hpp>
# include <Logger.hpp>
# include <utils.hpp>

#define MAX_EVENTS 10
#define MAX_CLIENTS 10

/// @brief The ServerInfo struct holds the configuration and state for a single server instance.
/// It manages the server socket, epoll instance, and various server settings like port, root directory, and error pages.
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

	/// @brief Constructor to initialize the ServerInfo with specific configurations.
	/// @param _port Port number the server listens on.
	/// @param _clientMaxBodySize Maximum body size for client requests.
	/// @param _index Default index file for the server.
	/// @param _name Name of the server.
	/// @param _root Root directory of the server.
	/// @param _host Hostname of the server.
	/// @param _defaultErrorPage Default error page to serve on errors.
	ServerInfo(int _port, int _clientMaxBodySize, std::string _index, 
				std::string _name, std::string _root, std::string _host, std::string _defaultErrorPage);
	~ServerInfo();
	
	/// @brief Initializes the server socket, binds it to the specified port, and sets up epoll for event handling.
	void InitInfo();

};

#endif
