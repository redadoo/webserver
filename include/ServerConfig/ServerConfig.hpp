#pragma once
#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

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

	/// @brief Finds the best matching `Location` object for a given request path by comparing the length of matching paths.
	/// @param requestPath The path of the incoming request to match against the defined locations.
	/// @return A pointer to the `Location` object that has the longest matching path. Returns `nullptr` if no matching location is found.
	const Location* FindMatchingLocation(const std::string& requestPath) const;

	/// @brief Constructs the full filesystem path from a relative path by combining it with the server's root directory.
	/// @param path The relative path to be combined with the server root.
	/// @return The resulting full path. If the relative path is empty or just "/", it returns the server root path. Otherwise, it appends the relative path to the server root.
	std::string GetFullPath(const std::string& path) const;

};

#endif // SERVERCONFIG_HPP
