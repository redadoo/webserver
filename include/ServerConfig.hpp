#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <string>
# include <stdint.h>

/// @brief Structure holding the server's configuration settings.
struct ServerConfig
{
	const uint16_t								serverPort;
	const std::string							host;
	const std::string							serverName;
	const std::string							serverRoot;
	const std::string 							indexPage;
	const std::string							clientMaxBody;
	const std::string 							errorPage;
	const std::string 							socketIp;

		/// @brief Constructor to initialize all server configuration parameters.
		/// @param _serverPort Port number for the server.
		/// @param _host Hostname or IP address.
		/// @param _serverName Name of the server.
		/// @param _serverRoot Root directory path.
		/// @param _indexPage Default index page.
		/// @param _clientMaxBody Maximum allowed body size for client requests.
		/// @param _errorPage Path to the error page.
		/// @param _socketIp IP address for the socket.
		ServerConfig(
			uint16_t _serverPort, 
			const std::string& _host, 
			const std::string& _serverName,
			const std::string& _serverRoot, 
			const std::string& _indexPage,
			const std::string& _clientMaxBody, 
			const std::string& _errorPage,
			const std::string& _socketIp
			)
			: 
			serverPort(_serverPort), 
			host(_host), 
			serverName(_serverName), 
			serverRoot(_serverRoot),
			indexPage(_indexPage), 
			clientMaxBody(_clientMaxBody), 
			errorPage(_errorPage), 
			socketIp(_socketIp) {}
};


#endif