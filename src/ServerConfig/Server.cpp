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
	EpollUtils::EpollAdd(epollFd, this->serverFd, EPOLLIN | EPOLLPRI);

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
	// if (IsMyClient(clientFd))
	// {
	// 	Logger::Log("Client alredy connected");
	// }
	// else
	// {
		this->AddClient(clientFd, ip, port);
		Logger::ClientLog(*this, GetClient(clientFd), " has been accepted!");
		EpollUtils::EpollAdd(epollFd, clientFd, EPOLLIN | EPOLLPRI);
	// }
	return (0);
}

void Server::ReadClientResponse(Client &client)
{
	const unsigned long long maxBodySize = serverConfig.clientMaxBody.ConvertToBytes();
	unsigned long long totalBytesRead = 0;
	ssize_t recvRet;
	char buffer[MAX_RESPONSE_SIZE];

	while (true)
	{
		recvRet = recv(client.clientFd, buffer, sizeof(buffer) - 1, 0);

		if (recvRet == 0)
			return;

		if (recvRet < 0)
			return CloseClientConnection(client);

		buffer[recvRet] = '\0';
		totalBytesRead += recvRet;

		if (totalBytesRead > maxBodySize)
		{
			Logger::Log("Client body size exceeded the limit: " + StringUtils::ToString(maxBodySize) + " bytes");
			throw WebServerException::HttpStatusCodeException(HttpStatusCode::PayloadTooLarge);
		}

		client.request.ParseMessage(buffer);

		Header::const_iterator it = client.request.header.find("Content-Length");
		if (it != client.request.header.end())
		{
			unsigned long long contentLength = StringUtils::StringToUnsignedLongLong(it->second);
			if (client.request.body.length() >= contentLength)
				break;
		}
		else
			break;
	}

	Logger::RequestLog(*this, client, client.request);
}

void Server::ProcessRequest(Client& client)
{

    const HttpMessage& request = client.request;
    Logger::Log("Processing request for path: " + request.startLine.path);

    std::string requestedPath = GetFullPath(request.startLine.path);
    Logger::Log("Full requested path: " + requestedPath);

    if (FileUtils::IsDirectory(requestedPath.c_str()))
		HandleDirectoryRequest(requestedPath);
	else if (!FileUtils::CheckFileExistence(requestedPath.c_str()))
		throw WebServerException::HttpStatusCodeException(HttpStatusCode::NotFound);
	else
		HandleFileRequest(requestedPath);

	response.SetContentLength();
	LogResponseHeaders();
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

void Server::HandleNotFound()
{
	Logger::Log("File not found, preparing 404 Not Found response");
	response.SetStatusCode(HttpStatusCode::NotFound);
	response.SetErrorBody(serverConfig);
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
