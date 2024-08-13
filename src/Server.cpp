# include <Server.hpp>
# include <Logger.hpp>
# include <EpollUtils.hpp>
# include <WebServerException.hpp>
# include <utils.hpp>
# include <unistd.h>

Server::Server (
	uint16_t _port, 
	const std::string & _clientMaxBodySize, 
	const std::string & _index,
	const std::string & _name, 
	const std::string & _root, 
	const std::string & _host,
	const std::string & _defaultErrorPage
	)
    : 	
	stop(false), 
	serverConfig(
			_port, 
			_host, 
			_name, 
			_root, 
			_index,
			_clientMaxBodySize, 
			_defaultErrorPage, 
			_host) 
			{
				serverFd = -1;
			}


void Server::InitSocket(int epollFd)
{
	int					ret;
	struct sockaddr_in	addr;
	socklen_t			addr_len;

	Logger::Log("Creating TCP socket...");

	this->serverFd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
	if (this->serverFd < 0)
		throw WebServerException::ExceptionErrno("socket() failed ", errno);
	
	addr_len = sizeof(addr);
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(this->serverConfig.serverPort);
	addr.sin_addr.s_addr = inet_addr(this->serverConfig.socketIp.c_str());

	ret = bind(this->serverFd, (struct sockaddr *)&addr, addr_len);
	if (ret < 0)
	{
		close(this->serverFd);
		throw WebServerException::ExceptionErrno("bind() failed ", errno);
	}
	
	ret = listen(this->serverFd, 10);
	if (ret < 0)
		throw WebServerException::ExceptionErrno("listen() failed ", errno);

	EpollUtils::EpollAdd(epollFd, this->serverFd, EPOLLIN | EPOLLPRI);
	
	Logger::Log(std::string("Listening on ") + this->serverConfig.socketIp + ":"
		+ utils::ToString(this->serverConfig.serverPort));
}

bool Server::IsMyClient(int clientFd)
{
	if (this->clients.find(clientFd) == clients.end()) 
		return false;
	
	return true;
}

Client &Server::GetClient(int clientFd)
{
	return clients.find(clientFd)->second;
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
	srcIp = utils::ConvertAddrNtop(&addr, srcIpBuffer);
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
		AddClient(clientFd, srcIp, srcPort);
		EpollUtils::EpollAdd(epollFd, clientFd, EPOLLIN | EPOLLPRI);
		Logger::ClientLog(*this, GetClient(clientFd), " has been accepted!");
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
	char buf[MAX_RESPONSE_SIZE];

	int ret_data = recv(client.clientFd, buf, MAX_RESPONSE_SIZE, 0);

	std::string msg(buf, buf + ret_data);
	msg =  "" + msg;

	Logger::EmptyLog(msg);
}

void Server::ParseClientResponse(Client &client)
{
	(void)client;
}

void Server::CloseClientConnection(Client &client, int epollFd)
{
	Logger::ClientLog(*this, client, "has been deleted ");
	EpollUtils::EpollDelete(epollFd, client.clientFd);
	close(client.clientFd);
	clients.erase(client.clientFd);
}

void Server::Init(int epollFd)
{
	InitSocket(epollFd);
}
