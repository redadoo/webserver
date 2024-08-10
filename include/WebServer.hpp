#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

# include <vector>
# include <Parser.hpp>
# include <Logger.hpp>
# include <WebServerException.hpp>
# include <WebServerSignal.hpp>

#define DEFAULT_CONFIG_FILE "config/default.conf"

/// @brief The WebServer class is responsible for initializing and starting the web server.
/// It manages server configurations and handles logging.
class WebServer
{
private:
    bool                    needToStop;
    
    std::vector<ServerInfo> serverInfo;

    void HandleClientEvent(int client_fd, uint32_t revents);
    void CloseConnection(ClientInfo client);
    int AcceptClient(int tcp_fd);

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
