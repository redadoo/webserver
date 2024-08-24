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

	Location &GetLastLocation();

	void CheckServerConfig();

	// Overload the << operator
	friend std::ostream& operator<<(std::ostream& os, const ServerConfig& sr);

	class ErrorPageNotFound : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Error page not found";
			}
	};

	class ClientBodySizeNotFound : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Client body size not found";
			}
	};

	class HostNotFound : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Host not found";
			}
	};

	class PortNotFound : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Port not found";
			}
	};
};

#endif // SERVERCONFIG_HPP
