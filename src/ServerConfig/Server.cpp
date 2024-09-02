#include <EpollUtils.hpp>
#include <Logger.hpp>
#include <Server.hpp>
#include <unistd.h>
#include <StringUtils.hpp>
#include <NetworkUtils.hpp>
#include <FIleUtils.hpp>
#include <WebServerException.hpp>
#include <HttpStatusCode.hpp>
#include <HttpMessage.hpp>
#include <HttpResponse.hpp>
#include <ClientBodySize.hpp>
#include <stdexcept>
#include <dirent.h>
#include <sys/stat.h>

//constructor

Server::Server() : serverConfig() {}


//private function

void Server::AddClient(int clientFd, std::string ip, uint16_t port)
{
	Client client(clientFd, ip, port);
	std::pair<int, Client> client_pair(clientFd, client);
	clients.insert(client_pair);
}

void Server::BuildResponse()
{
}

void Server::BuildErrorResponse()
{
}

//public function

void Server::InitSocket(int epollFd)
{
	int					ret;
	struct sockaddr_in	addr;
	socklen_t			addr_len;
	const int			enable = 1;

	Logger::Log("Creating TCP socket...");
	this->serverFd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
	if (this->serverFd < 0)
		throw WebServerException::ExceptionErrno("socket() failed ", errno);

	ret = setsockopt(this->serverFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
	if (ret < 0)
		throw WebServerException::ExceptionErrno("setsockopt() failed ", errno);

	addr_len = sizeof(addr);
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(this->serverConfig.serverPort.port);
	addr.sin_addr.s_addr = inet_addr(this->serverConfig.socketIp.c_str());

	ret = bind(this->serverFd, (struct sockaddr *)&addr, addr_len);
	if (ret < 0)
		throw WebServerException::ExceptionErrno("bind() failed ", errno);

	ret = listen(this->serverFd, 10);
	if (ret < 0)
		throw WebServerException::ExceptionErrno("listen() failed ", errno);

	EpollUtils::EpollAdd(epollFd, this->serverFd, EPOLLIN | EPOLLET);

	Logger::Log(std::string("Listening on ") + this->serverConfig.socketIp + ":"
		+ StringUtils::ToString(this->serverConfig.serverPort.port));
}

bool Server::IsMyClient(int clientFd)
{
	if (this->clients.find(clientFd) == clients.end())
		return false;
	return true;
}

Client &Server::GetClient(int clientFd)
{
	return (clients.find(clientFd)->second);
}

int Server::AcceptClient(int fd, int epollFd)
{
	int					clientFd;
	struct sockaddr_in	addr;
	socklen_t			addrLen;
	uint16_t			port;
	const char			*ip;
	char				ipBuffer[sizeof("xxx.xxx.xxx.xxx")];

	Logger::ServerLog(*this, "A client is trying to connecting to ");
	addrLen = sizeof(addr);
	memset(&addr, 0, sizeof(addr));
	clientFd = accept(fd, (struct sockaddr *)&addr, &addrLen);
	if (clientFd < 0)
	{
		if (errno == EAGAIN)
			return (0);
		Logger::LogWarning("error on accept(): ");
		return (-1);
	}

	port = ntohs(addr.sin_port);
	ip = NetworkUtils::ConvertAddrNtop(&addr, ipBuffer);
	if (!ip)
	{
		Logger::LogWarning("Cannot parse source address");
		close(clientFd);
		return (-1);
	}
	this->AddClient(clientFd, ip, port);
	Logger::ClientLog(*this, GetClient(clientFd), " has been accepted!");
	EpollUtils::EpollAdd(epollFd, clientFd, EPOLLIN | EPOLLET);
	return (0);
}

void Server::ReadClientResponse(Client &client)
{
	const unsigned long long maxBodySize = serverConfig.clientMaxBody.ConvertToBytes();
	ssize_t recvRet;
	char	buffer[MAX_RESPONSE_SIZE];

	recvRet = recv(client.clientFd, buffer, sizeof(buffer) - 1, 0);

	if (recvRet == 0)
		return;

	if (recvRet < 0)
		return CloseClientConnection(client);

	buffer[recvRet] = '\0';

	client.request.ParseMessage(buffer);

	Logger::RequestLog(*this, client, client.request);

	if (client.request.body.size() > maxBodySize)
	{
		Logger::Log("Client body size exceeded the limit: " + StringUtils::ToString(maxBodySize) + " bytes");
		throw WebServerException::HttpStatusCodeException(HttpStatusCode::PayloadTooLarge);
	}
}

void Server::ProcessRequest(Client& client, int redirectCount = 0)
{
	if (redirectCount > MAX_REDIRECT)
	{
		Logger::LogError("Too many redirects, aborting request");
		throw WebServerException::HttpStatusCodeException(HttpStatusCode::InternalServerError);
	}

	const Location* location = FindMatchingLocation(client.request.startLine.path);
	std::string requestedPath;
	std::string requestedFile;

	if (location)
	{
		if (location->ShouldRedirect())
		{
			SendRedirectResponse(client, location->redirect, redirectCount);
			return;
		}

		if (!location->IsMethodAllowed(client.request.startLine.httpMethod))
			throw WebServerException::HttpStatusCodeException(HttpStatusCode::MethodNotAllowed);

		requestedPath = location->GetFilePath(client.request.startLine.path, serverConfig.serverRoot);
		requestedFile = requestedPath.substr(0, requestedPath.length() - 1);

		if (!FileUtils::IsDirectory(requestedPath.c_str()) && !FileUtils::CheckFileExistence(requestedFile.c_str()))
		{
			Logger::Log("Requested path does not exist: " + requestedPath);
			throw WebServerException::HttpStatusCodeException(HttpStatusCode::NotFound);
		}
	}
	else
	{
		requestedPath = GetFullPath(client.request.startLine.path);
		requestedFile = requestedPath.substr(0, requestedPath.length() - 1);
	}

	Logger::Log("Processing request for path: " + client.request.startLine.path);
	Logger::Log("Full requested path: " + requestedPath);

	if (FileUtils::IsDirectory(requestedPath.c_str()))
	{
		if ((location && location->autoIndex) || (!location && serverConfig.autoIndex))
			HandleDirectoryListing(requestedPath, client);
		else
			HandleDirectoryRequest(requestedPath);
	}
	else if (!FileUtils::CheckFileExistence(requestedFile.c_str()))
		throw WebServerException::HttpStatusCodeException(HttpStatusCode::NotFound);
	else
		HandleFileRequest(requestedFile);

    // const HttpMessage& request = client.request;


    // if (FileUtils::IsDirectory(requestedPath.c_str()))
	// 	HandleDirectoryRequest(requestedPath);
	// else if (!FileUtils::CheckFileExistence(requestedPath.c_str()))
	// 	throw WebServerException::HttpStatusCodeException(HttpStatusCode::NotFound);
	// else
	// 	HandleFileRequest(requestedPath);

	response.SetContentLength();
	LogResponseHeaders();
	SendResponse(client);
}

const Location* Server::FindMatchingLocation(const std::string& requestPath) const
{
	const Location* bestMatch = NULL;
	size_t bestMatchLength = 0;
	Logger::Log("Finding matching location for path: " + requestPath);

	if (serverConfig.locations.empty())
	{
		Logger::Log("No locations defined, using default location");
		return NULL;
	}

	for (std::vector<Location>::const_iterator it = serverConfig.locations.begin(); it != serverConfig.locations.end(); ++it)
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

std::string Server::GetFullPath(const std::string& path)
{
    std::string fullPath = serverConfig.serverRoot;

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

void Server::HandleDirectoryListing(const std::string& path, Client& client)
{
	DIR* dir;
	struct dirent* entry;


	std::string displayPath = path;

	if (displayPath.substr(0, serverConfig.serverRoot.length()) == serverConfig.serverRoot)
		displayPath = displayPath.substr(serverConfig.serverRoot.length());

	std::string referer = client.request.referer;
	std::string pathStart = displayPath.substr(0, displayPath.find("/", 1));
	// while (referer.find(pathStart) == std::string::npos)
	// {
	// 	displayPath = displayPath.substr(displayPath.find("/", 1));
	// 	pathStart = displayPath.substr(0, displayPath.find("/", 1));
	// }



	if (displayPath.empty())
		displayPath = "/" + displayPath;


	std::string directoryContent = "<html><head><title>Index of " + displayPath + "</title></head><body>";
	directoryContent += "<h1>Index of " + displayPath + "</h1><hr><pre>";

	dir = opendir(path.c_str());
	if (!dir)
	{
		Logger::LogError("Failed to open directory: " + path);
		throw WebServerException::HttpStatusCodeException(HttpStatusCode::InternalServerError);
	}

	while ((entry = readdir(dir)) != NULL)
	{
		std::string name = entry->d_name;
		if (name != "." && name != "..")
		{
			std::string fullPath;
			fullPath = "<a href=\"" + client.request.referer + displayPath + "\">" + name + "</a>";


			std::string rootPath = path + name;
			struct stat fileStat;
			if (stat(rootPath.c_str(), &fileStat) < 0)
				continue;

			std::string fileSize = S_ISDIR(fileStat.st_mode) ? "-" : StringUtils::ToString(fileStat.st_size);
			std::string lastModified = StringUtils::FormatTime(fileStat.st_mtime);

			directoryContent += fullPath;
    		directoryContent += " (" + fileSize + ") " + lastModified + "</li>";
		}
	}

	closedir(dir);

	directoryContent += "</pre><hr></body></html>";

	response.SetStatusCode(HttpStatusCode::OK);
	response.SetBody(directoryContent);
	response.header["Content-Type"] = "text/html";
}

void Server::HandleDirectoryRequest(const std::string& path)
{
	Logger::Log("Requested path is a directory, looking for index file");
    bool indexFound = false;
	for (std::vector<std::string>::const_iterator it = serverConfig.indexPages.begin(); it != serverConfig.indexPages.end(); ++it)
	{
		std::string indexPath = path + "/" + *it;
		if (FileUtils::CheckFileExistence(indexPath.c_str()))
		{
			HandleFileRequest(indexPath);
			indexFound = true;
			break;
		}
	}
	if (!indexFound)
	{
		Logger::Log("No index file found in directory: " + path);
		throw WebServerException::HttpStatusCodeException(HttpStatusCode::Forbidden);
	}
}

void Server::HandleFileRequest(const std::string& path)
{
	Logger::Log("File found: " + path);
	std::string fileContent = FileUtils::ReadFile(path);
	if (fileContent.empty())
	{
		Logger::Log("Warning: File is empty or could not be read: " + path);
	}
	Logger::Log("File content length: " + StringUtils::ToString(static_cast<int>(fileContent.length())) + " bytes");

	response.SetStatusCode(HttpStatusCode::OK);
	response.body = fileContent;
	response.SetContentType(FileUtils::GetContentType(path));
}

void Server::LogResponseHeaders()
{
	Logger::Log("Response headers:");
	for (Header::const_iterator it = response.header.begin(); it != response.header.end(); ++it)
	{
		Logger::Log("  " + it->first + ": " + it->second);
	}
}

void Server::SendResponse(const Client& client)
{
    std::string responseStr = response.ToString();
    size_t totalBytesSent = 0;
    size_t totalLength = responseStr.length();

    while (totalBytesSent < totalLength)
    {
        ssize_t bytesSent = send(client.clientFd, responseStr.c_str() + totalBytesSent, totalLength - totalBytesSent, 0);

        if (bytesSent == -1)
        {

           Logger::LogError("Failed to send response: " + std::string(strerror(errno)));
        	return;

        }
        else if (bytesSent == 0)
        {
            Logger::Log("Connection closed by client");
            return;
        }

        totalBytesSent += bytesSent;
    }

    Logger::Log("Sent " + StringUtils::ToString(static_cast<int>(totalBytesSent)) + " bytes to client");
}

void Server::SendErrorResponse(const Client& client, HttpStatusCode::Code code)
{
	response.SetStatusCode(code);
	response.SetErrorBody(serverConfig);
	SendResponse(client);
}

void Server::SendRedirectResponse(Client& client, const CodePath& redirect, int redirectCount)
{
	response.SetStatusCode(redirect.code);
	response.header["Location"] = redirect.path;
	client.request.startLine.path = redirect.path;
	ProcessRequest(client, redirectCount + 1);
}


// void Server::SendResponse(const Client& client)
// {
//     std::string responseStr = St
//     ssize_t bytesSent = send(client.clientFd, responseStr.c_str(), responseStr.length(), 0);
//     if (bytesSent < 0)
//     {
//         Logger::LogError("Failed to send response: " + std::string(strerror(errno)));
//     }
//     else
//     {
//         Logger::Log("Sent " + StringUtils::ToString(static_cast<int>(bytesSent)) + " bytes to client");
//     }
// }


// void Server::SendResponse(const Client &client)
// {
// 	(void)client;
// 	// if (HttpStatusCode::isError(response.code))
// 	// 	BuildErrorResponse();
// 	// else
// 	// 	BuildResponse();

// 	// const char * str = response.c_str();

// 	// if (send(client.clientFd, str, response.size(), 0) < 0)
// 	// {
// 	// 	Logger::LogError("Failed to send response:");
// 	// 	throw std::invalid_argument("send() failed");
// 	// }

// 	// Logger::ResponseLog(*this,client, " ");
// }

void Server::CloseClientConnection(const Client &client)
{
	close(client.clientFd);
	this->clients.erase(client.clientFd);
	Logger::ClientLog(*this, client, "has been disconnected ");
}

void Server::CloseClientConnection(int clientFd)
{
	Client	client;

	if (IsMyClient(clientFd))
	{
		client = clients.find(clientFd)->second;
		close(client.clientFd);
		this->clients.erase(clientFd);
		Logger::ClientLog(*this, client, "has been disconnected ");
	}
}

std::ostream &operator<<(std::ostream &os, const Server &sr)
{
	os << "Server Configuration:\n" << sr.serverConfig;
	return os;
}
