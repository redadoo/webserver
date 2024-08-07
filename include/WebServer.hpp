#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

# include <vector>
# include <iostream>
# include <Lexer.hpp>
# include <Parser.hpp>
# include <sys/socket.h>
# include <netinet/in.h>
# include <sys/select.h>
# include <arpa/inet.h>
# include <Logger.hpp>

#define DEFAULT_CONFIG_FILE "config/default.conf"

/// @brief The WebServer class is responsible for initializing and starting the web server.
/// It manages server configurations and handles logging.
class WebServer
{
private:
    std::vector<ServerInfo> serverInfo;

public:
    /// @brief Starts the web server and begins listening for incoming connections.
    void StartServer();

    /// @brief Initializes the server configuration based on parsed data.
    void InitServer();

    /// @brief Default constructor, initializes the server using the default configuration file.
    WebServer();

    /// @brief Constructor that allows specifying a custom configuration file.
    /// @param filePath Path to the custom configuration file.
    WebServer(const char *filePath);
    ~WebServer();
};

#endif
