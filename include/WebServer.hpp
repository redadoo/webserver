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
	bool					needToStop;
	
	std::vector<Server>		servers;

	/// @brief Handles events for a specific client.
	void HandleClientEvent(Client &client, uint32_t revents, Server& Server);

    /// @brief Processes all active sockets and handles their events.
	void CheckSockets(int epollRet);

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
