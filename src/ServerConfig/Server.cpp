#include <EpollUtils.hpp>
#include <Logger.hpp>
#include <Server.hpp>
#include <StringUtils.hpp>
#include <NetworkUtils.hpp>
#include <FIleUtils.hpp>
#include <WebServerException.hpp>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <Cgi.hpp>
#include <stdexcept>
#include <string>
#include <stdint.h>
#include <sys/types.h>
//constructor

Server::Server() : serverConfig() {}

//private function

void Server::AddClient(int clientFd, std::string ip, uint16_t port)
{
	Client client(clientFd, ip, port);
	std::pair<int, Client> client_pair(clientFd, client);
	clients.insert(client_pair);
}

void Server::HandleDeleteRequest(const Client& client, const Location* location)
{
	if (!location || !location->IsMethodAllowed("DELETE"))
		throw WebServerException::HttpStatusCodeException(HttpStatusCode::MethodNotAllowed);

	std::string filePath = serverConfig.GetFullPath(client.request.startLine.path);
	if (FileUtils::CheckFileExistence(filePath.c_str()))
	{
		if (remove(filePath.c_str()) == 0)
		{
			response.SetStatusCode(HttpStatusCode::NoContent);
			response.SetBody("File deleted successfully");
		}
		else
			throw WebServerException::HttpStatusCodeException(HttpStatusCode::InternalServerError);
	}
	else
		throw WebServerException::HttpStatusCodeException(HttpStatusCode::NotFound);
}

void Server::HandlePostRequest(Client& client, const Location* location)
{
	if (!location || !location->IsMethodAllowed("POST"))
		throw WebServerException::HttpStatusCodeException(HttpStatusCode::MethodNotAllowed);

	std::string filePath = location->GetFilePath(client.request.startLine.path, serverConfig.serverRoot);

	if (filePath[filePath.length() - 1] == '/')
		filePath = filePath.substr(0, filePath.length() - 1);

	if (location->uploadEnable)
		HandleUploadRequest(client, location);
	else if (IsCgiRequest(StringUtils::GetScriptPath(filePath), location))
		HandleCgiRequest(client, filePath, location);
	else
		throw WebServerException::HttpStatusCodeException(HttpStatusCode::MethodNotAllowed);
}

void Server::HandleGetRequest(Client& client, const Location* location)
{
	std::string folderPath;
	if (location)
		folderPath = location->GetFilePath(client.request.startLine.path, serverConfig.serverRoot);
	else
		folderPath = serverConfig.GetFullPath(client.request.startLine.path);

	std::string filePath = (folderPath[folderPath.length() - 1] == '/') ? folderPath.substr(0, folderPath.length() - 1) : folderPath;
	if (location && IsCgiRequest(StringUtils::GetScriptPath(filePath), location))
		return (HandleCgiRequest(client, filePath, location));
	else if (FileUtils::IsDirectory(folderPath.c_str()))
	{
		if ((location && location->autoIndex) || (!location && serverConfig.autoIndex))
			HandleDirectoryListing(folderPath, client);
		else
			HandleDirectoryRequest(folderPath);
	}
	else if (!FileUtils::CheckFileExistence(filePath.c_str()))
		throw WebServerException::HttpStatusCodeException(HttpStatusCode::NotFound);
	else
		HandleFileRequest(filePath);

	response.SetContentLength();
	Logger::ResponseHeaderLog(response.header);
}

void Server::HandleUploadRequest(Client& client, const Location* location)
{
	std::string uploadPath;
	if (location && location->rootPath != "")
		uploadPath = location->rootPath.substr(0, location->rootPath.size() - 1) + location->path + "/" + location->uploadPath;
	else
		uploadPath = serverConfig.serverRoot.substr(0, serverConfig.serverRoot.size() - 1) + location->path + "/" + location->uploadPath;

	if (!FileUtils::CheckFileExistence(uploadPath.c_str()))
	{
		Logger::Log("Upload directory does not exist, creating it: " + uploadPath);
		if (mkdir(uploadPath.c_str(), 0777) == -1)
		{
			Logger::LogError("Failed to create upload directory: " + uploadPath);
			throw WebServerException::HttpStatusCodeException(HttpStatusCode::InternalServerError);
		}
	}

	std::string boundary = StringUtils::GetBoundary(client.request.header["Content-Type: "]);
	Logger::Log("Extracted boundary: " + boundary);
	std::vector<Body> parts = StringUtils::SplitMultipartData(client.request.body, boundary);
	Logger::Log("Extracted " + StringUtils::ToString(parts.size()) + " multipart data parts");
		
	for (size_t i = 0; i < parts.size(); ++i)
	{
		Logger::Log("Processing multipart data part " + StringUtils::ToString(i + 1) + " of " + StringUtils::ToString(parts.size()));
		
		std::string filename = parts[i].GetFileName();
		Ustring content = parts[i].GetFileContent();

		Logger::Log("Extracted content of length: " + StringUtils::ToString(content.size()));

		if (!filename.empty() && !content.empty())
		{
			std::string uploadFilePath = uploadPath + filename;
			if (FileUtils::WriteFile(uploadFilePath, content))
				Logger::Log("Uploaded file: " + filename + " to " + uploadFilePath);
			else
			{
				Logger::LogError("Failed to write uploaded file: " + filename);
				throw WebServerException::HttpStatusCodeException(HttpStatusCode::InternalServerError);
			}
		}
		else
			Logger::LogError("Skipping empty file upload part");
	}

	response.SetStatusCode(HttpStatusCode::OK);
	response.SetUploadBody();
}

void Server::HandleDirectoryListing(const std::string& path, Client& client)
{
	DIR* dir;
	struct dirent* entry;

	std::string displayPath = path;

	if (displayPath.substr(0, serverConfig.serverRoot.length()) == serverConfig.serverRoot)
		displayPath = displayPath.substr(serverConfig.serverRoot.length());

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
			std::string host = client.request.header["Host: "];
			std::string nameWithSlash = name + (FileUtils::IsDirectory((path + name).c_str()) ? "/" : "");
			std::string fullPath = "<a href=\"http://" + host + "/" + displayPath + nameWithSlash + "\">" + nameWithSlash + "</a>";


			std::string rootPath = path + name;
			struct stat fileStat;
			if (stat(rootPath.c_str(), &fileStat) < 0)
				continue;

			std::string fileSize = S_ISDIR(fileStat.st_mode) ? "-" : StringUtils::ToString(fileStat.st_size);
			std::string lastModified = StringUtils::FormatTime(fileStat.st_mtime);

			directoryContent += fullPath;
			directoryContent += " (" + fileSize + ") " + lastModified + "\n";
		}
	}

	closedir(dir);

	directoryContent += "</pre><hr></body></html>";

	response.SetStatusCode(HttpStatusCode::OK);
	response.SetBody(directoryContent);
	response.header["Content-Type: "] = "text/html";
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
	Ustring fileContent = FileUtils::ReadBinaryFile(path);
	Logger::Log("File content length: " + StringUtils::ToString(static_cast<int>(fileContent.size())) + " bytes");

	response.SetStatusCode(HttpStatusCode::OK);
	response.body = fileContent;
	response.SetContentType(FileUtils::GetContentType(path));
}

void Server::SendResponse(const Client& client)
{
	std::string responseStr = response.ToString();
	size_t totalBytesSent = 0;
	size_t totalLength = responseStr.length();

	Logger::Log("Attempting to send response of " + StringUtils::ToString(totalLength) + " bytes");
	while (totalBytesSent < totalLength)
	{
		ssize_t bytesSent = send(client.clientFd, responseStr.c_str() + totalBytesSent, totalLength - totalBytesSent, 0);
		if (bytesSent <= 0)
		{
			Logger::LogErrno();
			Logger::LogError("Failed to send response to client");
			break;
		}
		totalBytesSent += bytesSent;
	}
	Logger::Log("Sent " + StringUtils::ToString(static_cast<int>(totalBytesSent)) + " bytes to client");

}

void Server::SendRedirectResponse(Client& client, const CodePath& redirect, int redirectCount)
{
	response.SetStatusCode(redirect.code);
	response.header["Location: "] = redirect.path;
	client.request.startLine.path = redirect.path;
	ProcessRequest(client, redirectCount + 1);
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

	// if (!NetworkUtils::SetNonBlocking(this->serverFd))
	// {
	// 	Logger::LogWarning("Cannot set client socket to non-blocking mode");
	// 	close(this->serverFd);
	// 	throw WebServerException::ExceptionErrno("SetNonBlocking() failed ", errno);
	// }

	EpollUtils::EpollAdd(epollFd, this->serverFd, EPOLLIN | EPOLLOUT | EPOLLET);

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
		Logger::LogWarning("error on accept(): ");
		Logger::LogErrno();
		return (-1);
	}

	if (FileUtils::CheckFd(clientFd) < 0)
	{
		Logger::LogWarning("file descriptor invalid");
		Logger::LogErrno();
		return (-1);
	}

	if (!NetworkUtils::SetNonBlocking(clientFd))
	{
		Logger::LogWarning("Cannot set client socket to non-blocking mode");
		close(clientFd);
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
	EpollUtils::EpollAdd(epollFd, clientFd, EPOLLIN | EPOLLOUT | EPOLLET);
	return (0);
}

void Server::ReadClientRequest(Client &client)
{
	const unsigned long long 	maxBodySize = serverConfig.clientMaxBody.ConvertToBytes();
	ssize_t						recvRet;

	client.request.isBodyBinary = false;
	Logger::Log("read client message");

	while (!client.request.IsMessageComplete(maxBodySize, recvRet))
	{
		Ustring buffer(MAX_RESPONSE_CHUNK_SIZE);
		
		recvRet = recv(client.clientFd, buffer.data(), MAX_RESPONSE_CHUNK_SIZE, 0);
		
		Logger::Log(StringUtils::ToString(recvRet));
		
		if (recvRet < 0)
			return (this->CloseClientConnection(client));

		client.request.ParseMessage(buffer);
	}

	Logger::Log("finish read client message");
	Logger::RequestLog(*this, client, client.request);
}

void Server::ProcessRequest(Client& client, int redirectCount = 0)
{
	if (redirectCount > MAX_REDIRECT)
	{
		Logger::LogError("Too many redirects, aborting request");
		throw WebServerException::HttpStatusCodeException(HttpStatusCode::InternalServerError);
	}

	const Location* location = serverConfig.FindMatchingLocation(client.request.startLine.path);
	if (location && location->ShouldRedirect())
	{
		SendRedirectResponse(client, location->redirect, redirectCount);
		return;
	}

	if (client.request.startLine.httpMethod == "GET")
		HandleGetRequest(client, location);
	else if (client.request.startLine.httpMethod == "POST")
		HandlePostRequest(client, location);
	else if (client.request.startLine.httpMethod == "DELETE")
		HandleDeleteRequest(client, location);
	else
		throw WebServerException::HttpStatusCodeException(HttpStatusCode::MethodNotAllowed);
}

void Server::HandleCgiRequest(Client& client, const std::string& requestedPath, const Location* location)
{
	if (!location || location->cgiPath.empty() || location->cgiExtension.empty())
		throw WebServerException::HttpStatusCodeException(HttpStatusCode::InternalServerError);

	std::pair<std::string, std::string> pathAndQuery = StringUtils::SplitPathAndQuery(requestedPath);
	std::string scriptPath = pathAndQuery.first;

	Logger::Log("Executing CGI script: " + scriptPath);

	Cgi cgi(location->cgiPath, scriptPath);

	try
	{
		response = cgi.ProcessCgiRequest(*this, client,  client.request, serverConfig.host, serverConfig.serverPort.port);
		Logger::Log("CGI script executed successfully");
	}
	catch (const std::exception& ex)
	{
		Logger::LogError("Error executing CGI script: " + std::string(ex.what()));
		throw WebServerException::HttpStatusCodeException(HttpStatusCode::InternalServerError);
	}
}

bool Server::IsCgiRequest(const std::string& path, const Location* location) const
{
	return path.length() > 3 && path.substr(path.length() - 3) == ".py" && location->cgiExtension == ".py";
}

void Server::SendErrorResponse(const Client& client, HttpStatusCode::Code code)
{
	Logger::LogError("HTTP error occurred: " + HttpStatusCode::ReasonPhrase(code));
	try 
	{
		response.SetStatusCode(code);
		response.SetErrorBody(serverConfig);
		SendResponse(client);
	}
	catch (const std::invalid_argument &e) 
	{
		Logger::LogError("Unexpected exception in SendErrorResponse occurred: " + std::string(e.what()));
	}
}

void Server::CloseClientConnection(const Client &client)
{
	if (clients.find(client.clientFd) != clients.end())
	{
		close(client.clientFd);
		Logger::ClientLog(*this, client, "has been disconnected ");
		this->clients.erase(client.clientFd);
	}
}

void Server::CloseClientConnection(int clientFd)
{
	if (clientFd <= 0)
	{
		Logger::LogError("Attempted to close invalid client file descriptor: " + StringUtils::ToString(clientFd));
		return;
	}

	std::map<int, Client>::iterator it = clients.find(clientFd);
	if (it != clients.end())
	{
		Logger::Log("Closing connection for client " + StringUtils::ToString(clientFd));
		close(clientFd);
		clients.erase(it);
	}
	else
	{
		Logger::Log("Attempted to close connection for non-existent client " + StringUtils::ToString(clientFd));
	}
}

std::ostream &operator<<(std::ostream &os, const Server &sr)
{
	os << "Server Configuration:\n" << sr.serverConfig;
	return os;
}
