#include <ServerConfig.hpp>
#include <Logger.hpp>

ServerConfig::ServerConfig()
{
	this->autoIndex = false;
	this->serverPort.port = 0;
	this->isInitAutoindex = false;
}

Location &ServerConfig::GetLastLocation()
{
	if (locations.empty())
		throw std::invalid_argument("access to locations vector but no location found");

	return locations.back();
}

void ServerConfig::CheckServerConfig()
{
	if (this->serverPort.port == 0)
		throw std::invalid_argument("Port not found");

	if (this->host.empty())
		throw std::invalid_argument("Host not found");

	if (this->clientMaxBody.size == 0)
		throw std::invalid_argument("Client body size not found");

	if (this->errorPage.size() == 0)
		throw std::invalid_argument("Error page not found");
}

const Location* ServerConfig::FindMatchingLocation(const std::string& requestPath) const
{
	const Location* bestMatch = NULL;
	size_t bestMatchLength = 0;
	Logger::Log("Finding matching location for path: " + requestPath);

	if (locations.empty())
	{
		Logger::Log("No locations defined, using default location");
		return NULL;
	}

	for (std::vector<Location>::const_iterator it = locations.begin(); it != locations.end(); ++it)
	{
		if (it->MatchesPath(requestPath) && it->path.length() > bestMatchLength)
		{
			bestMatch = &(*it);
			bestMatchLength = it->path.length();
		}
	}

	if (bestMatch)
		Logger::Log("Found matching location: " + bestMatch->path);
	else
		Logger::Log("No matching location found");

	return bestMatch;
}

std::string ServerConfig::GetFullPath(const std::string& path) const
{
	std::string fullPath = serverRoot;

	if (!fullPath.empty() && fullPath[fullPath.length() - 1] != '/')
		fullPath += "/";

	if (path.empty() || path == "/")
		return fullPath;

	if (path[0] == '/')
		fullPath += path.substr(1);
	else
		fullPath += path;

	return fullPath;
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
