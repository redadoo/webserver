#ifndef Server_HPP
# define Server_HPP

# include <ServerConfig.hpp>
# include <Client.hpp>
# include <sys/epoll.h>
# include <netinet/in.h>
# include <map>
# include <vector>

# define MAX_RESPONSE_SIZE 2048


/// @brief Class representing server information and its operations.
class Server
{
private:
	/// @brief Initializes the server socket and binds it to the specified address and port.
	/// @param epollFd File descriptor for the epoll instance to which the server socket is added.
	void InitSocket(int epollFd);

    /// @brief Adds a new client to the server's client list.
    /// @param clientFd File descriptor for the client's socket.
    /// @param srcIp Source IP address of the client.
    /// @param srcPort Source port of the client.
	void AddClient(int clientFd,std::string srcIp, uint16_t srcPort);

public:
	int								serverFd;
	struct sockaddr_in 				serverAddress;

    ServerConfig					serverConfig;

    std::string                     response;
	std::map<int, Client>			clients;
	std::vector<std::string>		lastRequest;

	Server();

	/// @brief Initializes server information and sets up the socket.
	/// @param epollFd File descriptor for the epoll instance.
	void Init(int epollFd);

    /// @brief Checks if a client with the given file descriptor is connected to the server.
    /// @param clientFd File descriptor of the client to check.
    /// @return True if the client is connected, false otherwise.
    bool IsMyClient(int clientFd);

    /// @brief Retrieves the client object associated with the given file descriptor.
    /// @param clientFd File descriptor of the client.
    /// @return Reference to the Client object.
    Client &GetClient(int clientFd);

    /// @brief Accepts a new client connection and adds it to the server's client list.
    /// @param fd File descriptor of the server's listening socket.
    /// @param epollFd File descriptor for the epoll instance.
    /// @return 0 on success, -1 on failure.
    int AcceptClient(int fd, int epollFd);

    /// @brief Reads the client's response and stores it in the client's lastRequest.
    /// @param client Reference to the Client object.
    void ReadClientResponse(Client &client);

    /// @brief Parses the client's response.
    /// @param client Reference to the Client object.
    void ParseClientResponse(Client &client);

	void SendResponse(const Client &client);

    void BuildResponse();

    /// @brief Closes the connection to a client and removes it from the server's client list.
    /// @param client Reference to the Client object.
    void CloseClientConnection(const Client &client);

    /// @brief Closes the connection to a client and removes it from the server's client list.
    /// @param clientFd Reference to the Client filedesctiptor.
    void CloseClientConnection(int clientFd);
};

#endif
