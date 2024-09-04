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
#include <sys/wait.h>
#include <stdlib.h>
#include <poll.h>

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
	char	buffer[MAX_RESPONSE_SIZE];
	std::string fullRequest;
	ssize_t	recvRet;
	size_t	totalReceived = 0;
	size_t	contentLength = 0;
	bool	headersComplete = false;

	while (true)
	{
		recvRet = recv(client.clientFd, buffer, sizeof(buffer) - 1, 0);
		if (recvRet <= 0)
			break;

		buffer[recvRet] = '\0';
		fullRequest += buffer;
		totalReceived += recvRet;

		if (totalReceived > maxBodySize)
			throw WebServerException::HttpStatusCodeException(HttpStatusCode::PayloadTooLarge);

		if (!headersComplete)
		{
			size_t headerEnd = fullRequest.find("\r\n\r\n");
			if (headerEnd != std::string::npos)
			{
				headersComplete = true;
				size_t contentLengthPos = fullRequest.find("Content-Length: ");
				if (contentLengthPos != std::string::npos)
				{
					size_t endPos = fullRequest.find("\r\n", contentLengthPos);
					std::string contentLengthStr = fullRequest.substr(contentLengthPos + 16, endPos - (contentLengthPos + 16));
					contentLength = StringUtils::StringToSizeT(contentLengthStr);
				}
			}
		}

		if (headersComplete && totalReceived >= contentLength)
			break;
	}

	if (!fullRequest.empty())
	{
		client.request.ParseMessage(fullRequest);
		Logger::RequestLog(*this, client, client.request);
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

	SendResponse(client);
}

void Server::HandleDeleteRequest(Client& client, const Location* location)
{
	if (!location || !location->IsMethodAllowed("DELETE"))
		throw WebServerException::HttpStatusCodeException(HttpStatusCode::MethodNotAllowed);

	std::string filePath = GetFullPath(client.request.startLine.path);
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

	if (location->uploadEnable)
		HandleUploadRequest(client, location);
	else if (IsCgiRequest(GetScriptPath(client.request.startLine.path), location))
		HandleCgiRequest(client, GetScriptPath(client.request.startLine.path), location);
	else
		throw WebServerException::HttpStatusCodeException(HttpStatusCode::MethodNotAllowed);
}

void Server::HandleGetRequest(Client& client, const Location* location)
{
	std::string folderPath;
	if (location)
		folderPath = location->GetFilePath(client.request.startLine.path, serverConfig.serverRoot);
	else
		folderPath = GetFullPath(client.request.startLine.path);

	std::string filePath = (folderPath[folderPath.length() - 1] == '/') ? folderPath.substr(0, folderPath.length() - 1) : folderPath;
	if (location && IsCgiRequest(GetScriptPath(filePath), location))
	{
		HandleCgiRequest(client, filePath, location);
		return;
	}
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
	LogResponseHeaders();
}

std::pair<std::string, std::string> Server::SplitPathAndQuery(const std::string& path)
{
	size_t queryPos = path.find("?");
	if (queryPos != std::string::npos)
		return std::make_pair(path.substr(0, queryPos), path.substr(queryPos + 1));
	return std::make_pair(path, "");
}

void Server::HandleCgiRequest(Client& client, const std::string& requestedPath, const Location* location)
{
	if (!location || !location->cgiPath.empty() || !location->cgiExtension.empty())
		throw WebServerException::HttpStatusCodeException(HttpStatusCode::InternalServerError);

	std::pair<std::string, std::string> pathAndQuery = SplitPathAndQuery(requestedPath);
	std::string scriptPath = GetFullPath(pathAndQuery.first);
	std::string queryString = pathAndQuery.second;

	Logger::Log("Handling CGI request for script: " + scriptPath);

	int pipefd[2];
	pid_t pid;

	if (pipe(pipefd) == -1)
		throw WebServerException::HttpStatusCodeException(HttpStatusCode::InternalServerError);

	pid = fork();
	if (pid == -1)
		throw WebServerException::HttpStatusCodeException(HttpStatusCode::InternalServerError);

	if (pid == 0)
	{
		Logger::Log("Child process executing CGI script");
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);

		setenv("SCRIPT_FILENAME", scriptPath.c_str(), 1);
		setenv("REQUEST_METHOD", client.request.startLine.httpMethod.c_str(), 1);
		setenv("QUERY_STRING", queryString.c_str(), 1);
		setenv("CONTENT_LENGTH", StringUtils::ToString(client.request.body.size()).c_str(), 1);
		setenv("CONTENT_TYPE", client.request.header["Content-Type:"].c_str(), 1);

		if (client.request.startLine.httpMethod == "POST")
		{
			if (write(STDIN_FILENO, client.request.body.c_str(), client.request.body.size()) == -1)
			{
				Logger::LogError("Failed to write POST body to CGI script");
				exit(EXIT_FAILURE);
			}
		}

		std::string cgiInterpreter = location->cgiPath;
		execl(cgiInterpreter.c_str(), cgiInterpreter.c_str(), scriptPath.c_str(), NULL);
		Logger::LogError("Failed to execute CGI script: " + cgiInterpreter);
		exit(EXIT_FAILURE);
	}
	else
	{
		Logger::Log("Parent process waiting for CGI script to complete");
		close(pipefd[1]);
		char buffer[4096];
		std::string output;
		ssize_t bytesRead;

		while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer))) > 0)
		{
			output.append(buffer, bytesRead);
		}

		if (bytesRead == -1)
			Logger::LogError("Failed to read from CGI script");

		close(pipefd[0]);

		int status;
		waitpid(pid, &status, 0);

		if (WIFEXITED(status))
		{
			Logger::Log("Child process exited with status: " + StringUtils::ToString(WEXITSTATUS(status)));
			if (WEXITSTATUS(status) == 0)
			{
				size_t headerEnd = output.find("\r\n\r\n");
				if (headerEnd != std::string::npos)
				{
					Logger::Log("Parsing CGI response headers and body");
					response.body = output.substr(headerEnd + 4);
					std::istringstream headerStream(output.substr(0, headerEnd));
					std::string line;
					while (std::getline(headerStream, line) && !line.empty())
					{
						size_t colonPos = line.find(":");
						if (colonPos != std::string::npos)
						{
							std::string key = line.substr(0, colonPos);
							std::string value = line.substr(colonPos + 1);
							response.header[key] = value;
							Logger::Log("CGI header: " + key + ": " + value);
						}
					}
				}
				else
				{
					Logger::Log("No headers found in CGI response, using entire output as body");
					response.header["Content-Type"] = "text/html";
					response.body = output;
				}
				response.SetStatusCode(HttpStatusCode::OK);
				Logger::Log("CGI request processed successfully");
			}
		}
		else
			throw WebServerException::HttpStatusCodeException(HttpStatusCode::InternalServerError);
	}
}

bool Server::IsCgiRequest(const std::string& path, const Location* location) const
{
	return path.length() > 3 && path.substr(path.length() - 3) == ".py" && location->cgiExtension == ".py";
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

	std::string boundary = GetBoundary(client.request.header["Content-Type:"]);
	Logger::Log("Extracted boundary: " + boundary);
	std::vector<std::string> parts = SplitMultipartData(client.request.body, boundary);
	Logger::Log("Extracted " + StringUtils::ToString(parts.size()) + " multipart data parts");

	for (size_t i = 0; i < parts.size(); ++i)
	{
		Logger::Log("Processing multipart data part " + StringUtils::ToString(i + 1) + " of " + StringUtils::ToString(parts.size()));
		std::string filename = ExtractFilename(parts[i]);
		Logger::Log("Extracted filename: " + filename);
		std::string content = ExtractFileContent(parts[i]);
		Logger::Log("Extracted content: " + content);
		Logger::Log("Extracted content of length: " + StringUtils::ToString(content.length()));

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
	response.SetBody("File(s) uploaded successfully");
}

std::string Server::ExtractFileContent(const std::string& part) const
{
	size_t headerEnd = part.find("Content-Type:");
	if (headerEnd == std::string::npos)
	{
		Logger::LogError("Failed to find Content-Type header in multipart data");
		return "";
	}

	size_t contentStart = part.find("\n", headerEnd);
	if (contentStart == std::string::npos)
	{
		Logger::LogError("Failed to find start of content in multipart data");
		return "";
	}

	contentStart += 1;

	std::string content = part.substr(contentStart);

	content.erase(0, content.find_first_not_of("\t\r\n"));

	return content;
}

std::string Server::ExtractFilename(const std::string& part) const
{
	size_t filenamePos = part.find("filename=\"");
	if (filenamePos != std::string::npos)
	{
		size_t filenameStart = filenamePos + 10;
		size_t filenameEnd = part.find("\"", filenameStart);
		if (filenameEnd != std::string::npos)
		{
			std::string filename = part.substr(filenameStart, filenameEnd - filenameStart);
			Logger::Log("Extracted filename: " + filename);
			return filename;
		}
	}
	Logger::LogError("Failed to extract filename from multipart data part");
	return "";
}

std::string Server::GetBoundary(const std::string& contentType) const
{
	size_t boundaryPos = contentType.find("boundary=");
	if (boundaryPos != std::string::npos)
	{
		std::string boundary = contentType.substr(boundaryPos + 9);
		if (!boundary.empty() && boundary[0] == '"' && boundary[boundary.length() - 1] == '"')
			boundary = boundary.substr(1, boundary.length() - 2);
		return boundary;
	}
	return "";
}

std::vector<std::string> Server::SplitMultipartData(const std::string& data, const std::string& boundary) const
{
	std::vector<std::string> parts;
	std::string delimiter = "--" + boundary;
	std::string endDelimiter = delimiter + "--";

	size_t pos = 0;
	while (true)
	{
		size_t startPos = data.find(delimiter, pos);
		if (startPos == std::string::npos)
			break;

		startPos += delimiter.length();

		size_t endPos = data.find(delimiter, startPos);
		if (endPos == std::string::npos)
		{
			endPos = data.find(endDelimiter, startPos);
			if (endPos == std::string::npos)
				endPos = data.length();
		}

		std::string part = data.substr(startPos, endPos - startPos);
		part = StringUtils::Trim(part);
		if (!part.empty())
		{
			parts.push_back(part);
			Logger::Log("Found multipart data part of length: " + StringUtils::ToString(part.length()));
		}

		if (data.substr(endPos, endDelimiter.length()) == endDelimiter)
			break;

		pos = endPos;
	}

	Logger::Log("Total parts found: " + StringUtils::ToString(parts.size()));
	return parts;
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

std::string Server::GetScriptPath(const std::string& path)
{
	size_t queryPos = path.find("?");
	std::string scriptPath = (queryPos != std::string::npos) ? path.substr(0, queryPos) : path;
	return scriptPath;
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
			std::string host = client.request.header["Host:"];
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

		   Logger::LogError("Failed to send response");
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
	CloseClientConnection(client);
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
