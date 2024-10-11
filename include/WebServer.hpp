#pragma once
#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

#include <Parser.hpp>

# define MAX_EVENTS 32
# define TIMEOUT 3000

/// @brief The WebServer class is responsible for initializing and starting the web server.
/// It manages server configurations and handles logging.
class WebServer
{
private:
	std::vector<Server>		servers;
	struct epoll_event      events[32];
	int						epollFd;
	bool					needToStop;

	/// @brief Handles request and response from and to server e client.
	void HandleClientEvent(Client &client, Server& Server);

    /// @brief Processes all active sockets and handles their events.
	void CheckSockets(int epollRet);

	/// @brief Check if all server haves different port
	void CheckServerPort();

	/// @brief Closes all open file descriptors and cleans up resources.
	void CleanUpAll();

public:
	/// @brief Initializes the server based on parsed configuration data.
	void InitServer(const char *configFIle);

	/// @brief Starts the web server, entering the main event loop to handle incoming connections.
	void StartServer();

	WebServer();
	~WebServer();
};

#endif
