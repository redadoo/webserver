#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <string>
#include <vector>
#include <stdint.h>
#include <Location.hpp>
#include <CodePath.hpp>
#include <ClientBodySize.hpp>
#include <iostream>
#include <Port.hpp>

/// @brief Structure holding the server's configuration settings.
struct ServerConfig
{
	std::string 				host;
	std::vector<std::string> 	serverNames;
	std::string 				serverRoot;
	std::vector<std::string> 	indexPages;
	ClientBodySize 				clientMaxBody;
	std::vector<CodePath> 		errorPage;
	std::vector<Location> 		locations;
	bool						autoIndex;
	bool 						isInitAutoindex;
	Port 						serverPort;
	std::string 				socketIp;

	// Default constructor
	ServerConfig();

   	/// @brief Retrieves the last location in the locations list.
    /// @return Reference to the last Location object in the locations list.
	Location &GetLastLocation();

 	/// @brief Checks the validity of the server configuration.
    /// @throws std::invalid_argument If any required configuration value is missing or invalid.
	void CheckServerConfig();

	// Overload the << operator
	friend std::ostream& operator<<(std::ostream& os, const ServerConfig& sr);
};

#endif // SERVERCONFIG_HPP
