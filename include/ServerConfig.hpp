#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <string>
#include <vector>
#include <stdint.h>
#include <Location.hpp>
#include <CodePath.hpp>
#include <ClientBodySize.hpp>
#include <iostream> // Include for std::ostream

/// @brief Structure holding the server's configuration settings.
struct ServerConfig
{
	std::string host;
	std::vector<std::string> serverNames;
	std::string serverRoot;
	std::vector<std::string> indexPages;
	ClientBodySize clientMaxBody;
	std::vector<CodePath> errorPage;
	std::vector<Location> locations;
	bool autoIndex;
	bool isInitAutoindex;
	uint16_t serverPort;
	std::string socketIp;

	// Default constructor
	ServerConfig();

	// Overload the << operator
	friend std::ostream& operator<<(std::ostream& os, const ServerConfig& sr)
	{
		os << "Host: " << sr.host << "\n";
		os << "Server Names: ";
		for (size_t i = 0; i < sr.serverNames.size(); ++i)
		{
			os << sr.serverNames[i];
			if (i < sr.serverNames.size() - 1)
				os << ", ";
		}
		os << "\nServer Root: " << sr.serverRoot << "\n";
		os << "Index Pages: ";
		for (size_t i = 0; i < sr.indexPages.size(); ++i)
		{
			os << sr.indexPages[i];
			if (i < sr.indexPages.size() - 1)
				os << ", ";
		}
		os << "\nClient Max Body Size: " << sr.clientMaxBody << "\n"; // Assuming ClientBodySize has its own overloaded <<
		os << "Error Pages: ";
		for (size_t i = 0; i < sr.errorPage.size(); ++i)
		{
			os << sr.errorPage[i];
			if (i < sr.errorPage.size() - 1)
				os << "; ";
		}
		os << "\nLocations: \n";
		for (size_t i = 0; i < sr.locations.size(); ++i)
		{
			os << sr.locations[i] << "\n"; // Assuming Location has its own overloaded <<
		}
		os << "Auto Index: " << (sr.autoIndex ? "Enabled" : "Disabled") << "\n";
		os << "Is Init Autoindex: " << (sr.isInitAutoindex ? "Enabled" : "Disabled") << "\n";
		os << "Server Port: " << sr.serverPort << "\n";
		os << "Socket IP: " << sr.socketIp << "\n";
		return os;
	}
};

#endif // SERVERCONFIG_HPP
