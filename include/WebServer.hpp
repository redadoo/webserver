#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

# include <vector>
# include <Parser.hpp>
# include <Logger.hpp>
# include <WebServerException.hpp>
# include <WebServerSignal.hpp>
# include "unistd.h"

# define DEFAULT_CONFIG_FILE "config/default.conf"

/// @brief The WebServer class is responsible for initializing and starting the web server.
/// It manages server configurations and handles logging.
class WebServer
{
private:
    int                     epollFd;
	int					    err;
	int					    maxevents;
	int					    epoll_ret;
	int					    epoll_fd;
	int					    fd;
    struct epoll_event      events[32];
    int                     timeout;
    bool                    needToStop;
    
    std::vector<ServerInfo> serverInfos;

    
    void CheckSockets();
    void HandleClientEvent(int client_fd, uint32_t revents, const ServerInfo& serverInfo);
    void CloseConnection(ClientInfo client, const ServerInfo& serverInfo);
    int AcceptClient(int tcp_fd, const ServerInfo& serverInfo);

public:
    /// @brief Starts the web server and begins listening for incoming connections.
    void StartServer();

    /// @brief Initializes the server configuration based on parsed data.
    void InitServer();

    /// @brief Constructor that allows specifying a custom configuration file.
    /// @param filePath Path to the custom configuration file.
    WebServer(const char *filePath);

    WebServer();
    ~WebServer();
};

#endif
