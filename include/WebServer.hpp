#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

# include <vector>
# include <Parser.hpp>
# include "unistd.h"
# include <Client.hpp>

# define DEFAULT_CONFIG_FILE "config/default.conf"
# define MAX_EVENTS 32
# define TIMEOUT 3000

/// @brief The WebServer class is responsible for initializing and starting the web server.
/// It manages server configurations and handles logging.
class WebServer
{
private:
	int						epollFd;
	struct epoll_event      events[32];
	bool                    needToStop;
	
	std::vector<Server>		servers;

	/// @brief Handles events for a specific client file descriptor.
	/// @param client_fd File descriptor of the client.
	/// @param revents The events associated with the client fd.
	/// @param Server Configuration and state of the server handling this client.
	void HandleClientEvent(Client &client, uint32_t revents, Server& Server);

	/// @brief Closes the connection with a client and removes the client file descriptor from epoll.
	/// @param client Reference to the client info structure.
	/// @param Server Configuration and state of the server handling this client.
	void CloseConnection(Client &client, Server& Server);

	/// @brief Accepts a new client connection on the specified socket.
	/// @param tcp_fd File descriptor of the listening socket.
	/// @param Server Configuration and state of the server handling this client.
	/// @return Returns 0 on success, -1 on failure.
	int AcceptClient(int tcp_fd, Server &Server);

    /// @brief Processes all active sockets and handles their events.
	void CheckSockets(int epollRet);

	/// @brief 
	void ReadResponse(Client& client, Server &server);

	/// @brief Closes all open file descriptors and cleans up resources.
	void CleanUpAll();

public:
	/// @brief Starts the web server, entering the main event loop to handle incoming connections.
	void StartServer();

	/// @brief Initializes the server based on parsed configuration data.
	void InitServer();

	/// @brief Constructor that allows specifying a custom configuration file.
	/// @param filePath Path to the custom configuration file.
	WebServer(const char *filePath);

	WebServer();
	~WebServer();
};

#endif
