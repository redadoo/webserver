#ifndef SERVERINFO_HPP
# define SERVERINFO_HPP

# include <ServerConfig.hpp>
# include <ClientInfo.hpp>
# include <sys/epoll.h>
# include <netinet/in.h>
# include <map>

/// @brief Class representing server information and its operations.
class ServerInfo
{
private:
	/// @brief Initializes the server socket and binds it to the specified address and port.
	/// @param epollFd File descriptor for the epoll instance to which the server socket is added.
	void InitSocket(int epollFd);

public:
	int								serverFd;
	struct sockaddr_in 				serverAddress;
	bool							stop;
	ServerConfig					serverConfig;
	std::map<int, ClientInfo>	    clientsInfo;

	/// @brief Constructor to initialize server information and configuration.
	/// @param _port Server port number.
	/// @param _clientMaxBodySize Maximum allowed client request body size.
	/// @param _index Default index page.
	/// @param _name Server name.
	/// @param _root Server root directory.
	/// @param _host Hostname or IP address.
	/// @param _defaultErrorPage Path to the default error page.
	ServerInfo(int _port, const std::string &_clientMaxBodySize, const std::string & _index, 
				const std::string & _name, const std::string & _root, const std::string & _host, const std::string & _defaultErrorPage);
	
	/// @brief Initializes server information and sets up the socket.
	/// @param epollFd File descriptor for the epoll instance.
	void InitInfo(int epollFd);
};

#endif
