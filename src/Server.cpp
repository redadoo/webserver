# include <EpollUtils.hpp>
# include <Logger.hpp>
# include <Server.hpp>
# include <unistd.h>
# include <StringUtils.hpp>
# include <NetworkUtils.hpp>

Server::Server() {}

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
	addr.sin_port = htons(this->serverConfig.serverPort);
	addr.sin_addr.s_addr = inet_addr(this->serverConfig.socketIp.c_str());
	ret = bind(this->serverFd, (struct sockaddr *)&addr, addr_len);
	if (ret < 0)
		throw WebServerException::ExceptionErrno("bind() failed ", errno);

	ret = listen(this->serverFd, 10);
	if (ret < 0)
		throw WebServerException::ExceptionErrno("listen() failed ", errno);
	EpollUtils::EpollAdd(epollFd, this->serverFd, EPOLLIN | EPOLLPRI);

	Logger::Log(std::string("Listening on ") + this->serverConfig.socketIp + ":"
		+ StringUtils::ToString(this->serverConfig.serverPort));
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
	uint16_t			srcPort;
	const char			*srcIp;
	char				srcIpBuffer[sizeof("xxx.xxx.xxx.xxx")];

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

	srcPort = ntohs(addr.sin_port);
	srcIp = NetworkUtils::ConvertAddrNtop(&addr, srcIpBuffer);
	if (!srcIp)
	{
		Logger::LogWarning("Cannot parse source address");
		close(clientFd);
		return (-1);
	}
	if (IsMyClient(clientFd))
	{
		Logger::Log("Client alredy connected");
	}
	else
	{
		this->AddClient(clientFd, srcIp, srcPort);
		Logger::ClientLog(*this, GetClient(clientFd), " has been accepted!");
		EpollUtils::EpollAdd(epollFd, clientFd, EPOLLIN | EPOLLPRI);
	}
	return (0);
}

void Server::AddClient(int clientFd, std::string srcIp, uint16_t srcPort)
{
	Client client(clientFd, srcIp, srcPort);
	std::pair<int, Client> client_pair(clientFd, client);
	clients.insert(client_pair);
}

void Server::ReadClientResponse(Client &client)
{
	ssize_t	recv_ret;

	recv_ret = 1;
	client.lastRequest.clear();
	while (recv_ret > 0)
	{
		char			buffer[1024];

		recv_ret = recv(client.clientFd, buffer, sizeof(buffer), MSG_DONTWAIT);
		if (recv_ret == 0)
			return CloseClientConnection(client);

		if (recv_ret < 0)
		{
			if (errno == EAGAIN)
				break ;
			WebServerException::ExceptionErrno("recv(): ", errno);
			return (CloseClientConnection(client));
		}
		Logger::StartRequestLog(*this, client);

		buffer[recv_ret] = '\0';
		if (buffer[recv_ret - 1] == '\n')
			buffer[recv_ret - 1] = '\0';
		client.lastRequest.push_back(buffer);

		Logger::RequestLog(buffer);
	}
}

void Server::ParseClientResponse(Client &client)
{
		(void)client;

	for (size_t i = 0; i < client.lastRequest.size(); i++)
	{
		std::cout << client.lastRequest[i] << std::endl;
	}
	std::string line;
	std::istringstream responseStream(client.lastRequest[0]);

	std::getline(responseStream, line);

	std::istringstream lineStream(line);
	lineStream>>client.httpMethod>>client.path>>client.httpVersion;



	if(client.httpMethod != "GET" && client.httpMethod != "POST" && client.httpMethod != "DELETE")
	throw BadResponse();

	if(client.path.length() > 1024)
	throw BadResponse();

	if(client.path.find("../") != std::string::npos || client.path == ".." )
	throw BadResponse();

	if(client.httpVersion != "HTTP/1.1")
		throw BadResponse();

	if (client.httpMethod == "GET" || client.httpMethod == "DELETE")
		return;

	while (std::getline(responseStream, line) && !line.empty()) {
    	if (line.find("Content-Type") != std::string::npos && client.contentType.empty())
		{
			int i;
			std::string	tmp;
			while(line[i] != ' ')
				i++;
			while(line[i])
			{
				tmp += line[i];
			}
			client.contentType = tmp;
		}
		if (line.find("Content-Lenght") != std::string::npos && client.contentLenght != -1)
		{
			std::string	numberString;
			for (size_t i = 0; i < line.length(); i++)
			{
				if(isdigit(line[i]))
					numberString += line[i];
			}
			client.contentLenght = StringUtils::StrintToInt(numberString);
		}
    }


}

void Server::SendResponse(const Client &client)
{
	Logger::StartResponseLog(*this,client);

	BuildResponse();
	if (send(client.clientFd, response.c_str(), response.size(), 0) < 0)
	{
		Logger::LogError("Failed to send response: "
			+ std::string(strerror(errno)));
		throw WebServerException::ExceptionErrno("send() failed", errno);
	}

	Logger::ResponseLog(response);
}

void Server::BuildResponse()
{
	response.append("HTTP/1.1 200 OK\n");
	response.append("Content-Length: 702\n");
	response.append("Content-Type: text/html\n\n");

  	std::ifstream ifs("web-page/index.html");
  	std::string content( (std::istreambuf_iterator<char>(ifs) ),
					   (std::istreambuf_iterator<char>()    ) );

	response.append(content);
}

void Server::CloseClientConnection(const Client &client)
{
	close(client.clientFd);
	Logger::ClientLog(*this, client, "has been disconnected ");
	clients.erase(client.clientFd);
}

void Server::CloseClientConnection(int clientFd)
{
	Client	client;

	if (IsMyClient(clientFd))
	{
		client = clients.find(clientFd)->second;
		close(client.clientFd);
		Logger::ClientLog(*this, client, "has been disconnected ");
		clients.erase(client.clientFd);
	}
}

void Server::Init(int epollFd)
{
	InitSocket(epollFd);
}
