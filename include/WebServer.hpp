#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

# include <vector>
# include <Parser.hpp>
# include "unistd.h"
# include <ClientInfo.hpp>

# define DEFAULT_CONFIG_FILE "config/default.conf"
# define MAX_EVENTS 32
# define TIMEOUT 3000

/// @brief The WebServer class is responsible for initializing and starting the web server.
/// It manages server configurations and handles logging.
class WebServer
{
private:
	int						epollFd;
	int					    epoll_ret;
	int					    fd;
	struct epoll_event      events[32];
	bool                    needToStop;
	
	std::vector<ServerInfo> serverInfos;

	/// @brief Handles events for a specific client file descriptor.
	/// @param client_fd File descriptor of the client.
	/// @param revents The events associated with the client fd.
	/// @param serverInfo Configuration and state of the server handling this client.
	void HandleClientEvent(int client_fd, uint32_t revents, const ServerInfo& serverInfo);

	/// @brief Closes the connection with a client and removes the client file descriptor from epoll.
	/// @param client Reference to the client info structure.
	/// @param serverInfo Configuration and state of the server handling this client.
	void CloseConnection(ClientInfo &client, const ServerInfo& serverInfo);

	/// @brief Accepts a new client connection on the specified socket.
	/// @param tcp_fd File descriptor of the listening socket.
	/// @param serverInfo Configuration and state of the server handling this client.
	/// @return Returns 0 on success, -1 on failure.
	int AcceptClient(int tcp_fd, ServerInfo &serverInfo);

    /// @brief Processes all active sockets and handles their events.
	void CheckSockets();

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
