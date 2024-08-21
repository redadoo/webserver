#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <string>
# include <stdint.h>
# include <Location.hpp>
# include <CodePath.hpp>
# include <ClientBodySize.hpp>

/// @brief Structure holding the server's configuration settings.
struct ServerConfig
{
	std::string								host;
	std::vector<std::string>				serverNames;
	std::string								serverRoot;
	std::vector<std::string> 				indexPages;
	ClientBodySize							clientMaxBody;
	std::vector<CodePath>					errorPage;
	std::vector<Location>					locations;
	bool									autoIndex;
	bool									isInitAutoindex;
	uint16_t								serverPort;
	std::string								socketIp;

	ServerConfig();
};


#endif
