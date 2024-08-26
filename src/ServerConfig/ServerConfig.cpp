#include <ServerConfig.hpp>

ServerConfig::ServerConfig()
{
	this->serverPort.port = 0;
	this->isInitAutoindex = false;
}

Location &ServerConfig::GetLastLocation()
{
	return (locations[locations.size() - 1]);
}

void ServerConfig::CheckServerConfig()
{
	if (this->serverPort.port == 0)
		throw std::invalid_argument("Port not found");

	if (this->host.empty())
		throw std::invalid_argument("Host not found");

	if (this->clientMaxBody.size.empty())
		throw std::invalid_argument("Client body size not found");

	if (this->errorPage.size() == 0)
		throw std::invalid_argument("Error page not found");
}

std::ostream &operator<<(std::ostream &os, const ServerConfig &sr)
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
	os << "\nClient Max Body Size: " << sr.clientMaxBody << "\n";
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
		os << sr.locations[i] << "\n";
	}
	os << "Auto Index: " << (sr.autoIndex ? "Enabled" : "Disabled") << "\n";
	os << "Is Init Autoindex: " << (sr.isInitAutoindex ? "Enabled" : "Disabled") << "\n";
	os << "Server Port: " << sr.serverPort.port << "\n";
	os << "Socket IP: " << sr.socketIp << "\n";
	return (os);
}