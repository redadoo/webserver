#ifndef Server_HPP
# define Server_HPP

# include <ServerConfig.hpp>
# include <Client.hpp>
# include <sys/epoll.h>
# include <netinet/in.h>
# include <map>
# include <vector>

# define MAX_RESPONSE_SIZE 4096


/// @brief Class representing server information and its operations.
class Server
{
private:
	/// @brief Initializes the server socket and binds it to the specified address and port.
	/// @param epollFd File descriptor for the epoll instance to which the server socket is added.
	void InitSocket(int epollFd);

	void AddClient(int clientFd,std::string srcIp, uint16_t srcPort);

public:
	int								serverFd;
	struct sockaddr_in 				serverAddress;
	bool							stop;
	ServerConfig					serverConfig;
	std::map<int, Client>			clients;
	std::vector<std::string>		lastResponse;

	/// @brief Constructor to initialize server information and configuration.
	/// @param _port Server port number.
	/// @param _clientMaxBodySize Maximum allowed client request body size.
	/// @param _index Default index page.
	/// @param _name Server name.
	/// @param _root Server root directory.
	/// @param _host Hostname or IP address.
	/// @param _defaultErrorPage Path to the default error page.
	Server(uint16_t _port, const std::string &_clientMaxBodySize, const std::string & _index, 
				const std::string & _name, const std::string & _root, const std::string & _host, const std::string & _defaultErrorPage);
	
	/// @brief Initializes server information and sets up the socket.
	/// @param epollFd File descriptor for the epoll instance.
	void Init(int epollFd);

	bool IsMyClient(int clientFd);

	Client &GetClient(int clientFd);

	int  AcceptClient(int fd, int epollFd);
	void ReadClientResponse(Client &client);
	void ParseClientResponse(Client &client);
	void CloseClientConnection(Client &client, int epollFd);
};

#endif
