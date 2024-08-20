#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <string>
# include <stdint.h>
# include "Parser.hpp"

enum Unit;
struct Location;
struct CodePath;
struct ClientBodySize;



/// @brief Structure holding the server's configuration settings.
struct ServerConfig
{
	const uint16_t								serverPort;
	const std::string							host;
	const std::vector<std::string>				serverNames;
	const std::string							serverRoot;
	const std::vector<std::string> 				indexPages;
	const ClientBodySize						clientMaxBody;
	const std::vector<CodePath>					errorPage;
	const bool									autoIndex;
	const std::vector<Location>					locations;

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
			uint16_t						_serverPort,
			const std::string&				_host,
			const std::vector<std::string>& _serverNames,
			const std::string&				_serverRoot,
			const std::vector<std::string>& _indexPages,
			const ClientBodySize&			_clientMaxBody,
			std::vector<CodePath>&			_errorPage,
			bool 							_autoIndex,
			std::vector<Location>&			_locations
			)
			:
			serverPort(_serverPort),
			host(_host),
			serverNames(_serverNames),
			serverRoot(_serverRoot),
			indexPages(_indexPages),
			clientMaxBody(_clientMaxBody),
			errorPage(_errorPage),
			autoIndex(_autoIndex),
			locations(_locations) {}
};


#endif
