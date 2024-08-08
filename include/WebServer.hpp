#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

# include <vector>
# include <iostream>
# include <Lexer.hpp>
# include <Parser.hpp>
# include <Logger.hpp>
# include <unistd.h>
# include <cstring>

#define DEFAULT_CONFIG_FILE "config/default.conf"



// struct tcp_state {
//     bool                    stop;
//     int                     tcp_fd;
//     int                     epoll_fd;
//     uint16_t                client_c;
//     std::vector<ServerInfo> ServerInfo;

//     /*
//      * Map the file descriptor to client_slot array index
//      * Note: We assume there is no file descriptor greater than 10000.
//      *
//      * You must adjust this in production.
//      */
//     uint32_t            client_map[10000];
// };

/// @brief The WebServer class is responsible for initializing and starting the web server.
/// It manages server configurations and handles logging.
class WebServer
{
private:
    bool                    needToStop;
    
    std::vector<ServerInfo> serverInfo;

public:
    /// @brief Starts the web server and begins listening for incoming connections.
    void StartServer();

    /// @brief Initializes the server configuration based on parsed data.
    void InitServer();

    /// @brief Tokenize and parse configuration file
    void ParseData(const char *fileConf);

    /// @brief Constructor that allows specifying a custom configuration file.
    /// @param filePath Path to the custom configuration file.
    WebServer(const char *filePath);

    WebServer();
    ~WebServer();
};

#endif
