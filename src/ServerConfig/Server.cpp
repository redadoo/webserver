#include <EpollUtils.hpp>
#include <Logger.hpp>
#include <Server.hpp>
#include <unistd.h>
#include <StringUtils.hpp>
#include <NetworkUtils.hpp>
#include <WebServerException.hpp>
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
	if (IsMyClient(clientFd))
	{
		Logger::Log("Client alredy connected");
	}
	else
	{
		this->AddClient(clientFd, ip, port);
		Logger::ClientLog(*this, GetClient(clientFd), " has been accepted!");
		EpollUtils::EpollAdd(epollFd, clientFd, EPOLLIN | EPOLLPRI);
	}
	return (0);
}

void Server::ReadClientResponse(Client &client)
{
	ssize_t	recvRet;

	char			buffer[MAX_RESPONSE_SIZE];

	recvRet = recv(client.clientFd, buffer, sizeof(buffer), MSG_DONTWAIT);

	if (recvRet == 0)
		return;

	if (recvRet < 0)
		return CloseClientConnection(client);

	buffer[recvRet] = '\0';
	if (buffer[recvRet - 1] == '\n')
		buffer[recvRet - 1] = '\0';

	client.request.ParseMessage(buffer);

	Logger::RequestLog(*this, client, buffer);
}

void Server::SendResponse(const Client &client)
{
	(void)client;
	// if (HttpStatusCode::isError(response.code))
	// 	BuildErrorResponse();
	// else
	// 	BuildResponse();

	// const char * str = response.c_str();

	// if (send(client.clientFd, str, response.size(), 0) < 0)
	// {
	// 	Logger::LogError("Failed to send response:");
	// 	throw std::invalid_argument("send() failed");
	// }

	// Logger::ResponseLog(*this,client, " ");
}

void Server::CloseClientConnection(const Client &client)
{
	close(client.clientFd);
	Logger::ClientLog(*this, client, "has been disconnected ");
}

void Server::CloseClientConnection(int clientFd)
{
	Client	client;

	if (IsMyClient(clientFd))
	{
		client = clients.find(clientFd)->second;
		close(client.clientFd);
		Logger::ClientLog(*this, client, "has been disconnected ");
	}
}

std::ostream &operator<<(std::ostream &os, const Server &sr)
{
	os << "Server Configuration:\n" << sr.serverConfig;
	return os;
}