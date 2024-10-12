#pragma once

#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

#include <Server.hpp>

# define MAX_EVENTS 32 // Maximum number of events to be returned by epoll_wait
# define TIMEOUT 3000 // Maximum wait time (in milliseconds) for epoll_wait

/// @brief The WebServer class is responsible for initializing and starting multiple web servers.
/// It manages server configurations and handles logging.
class WebServer
{
public:

	/// @brief Default constructor
	WebServer();

	/// @brief Destructor cleans up resources (e.g., file descriptors).
	~WebServer();

	/// @brief Parses the configuration file, checks server ports, sets up signal handling, and initializes epoll.
	/// @param configFile name of the webserver configFile
	void InitServer(const char *configFile);

	/// @brief Starts the main event loop to handle incoming client connections and events.
	void StartServer();

private:
	std::vector<Server>		servers;
	struct epoll_event      events[32];
	int						epollFd;
	bool					needToStop;

	/// @brief Handles requests from a client, processes them, and sends responses.
	/// @param client client who send event
	/// @param Server server that manages the client 
	void HandleClientEvent(Client &client, Server& Server);

	/// @brief Handles events for all active sockets (client and server).
	/// @param epollRet epoll_wait return value
	void CheckSockets(int epollRet);

	/// @brief Ensures that all servers have unique port assignments to avoid conflicts.
	void CheckServerPort();

	/// @brief Cleans up by closing all open server file descriptors and the epoll file descriptor.
	void CleanUpAll();
};

#endif // !WEBSERVER_HPP
