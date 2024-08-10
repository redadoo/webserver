#include <ServerInfo.hpp>

void ServerInfo::InitEpoll()
{
	Logger::Log("Initializing epoll_fd...");
	this->epollFd = epoll_create1(0);
	if (this->epollFd < 0)
		throw WebServerException::ExceptionErrno("epoll_create() failed ",
			errno);
}

void ServerInfo::InitSocket()
{
	int					ret;
	struct sockaddr_in	addr;
	socklen_t			addr_len;

	addr_len = sizeof(addr);
	Logger::Log("Creating TCP socket...");
	this->serverFd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
	if (this->serverFd < 0)
		throw WebServerException::ExceptionErrno("socket() failed ", errno);
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
	/*
		* Add `tcp_fd` to epoll monitoring.
		*
		* If epoll returned tcp_fd in `events` then a client is
		* trying to connect to us.
		*/
	EpollUtils::EpollAdd(this->epollFd, this->serverFd, EPOLLIN | EPOLLPRI);
	Logger::Log(std::string("Listening on ") + this->serverConfig.socketIp + ":"
		+ utils::ToString(this->serverConfig.serverPort));
}

ServerInfo::ServerInfo(int _port, const std::string & _clientMaxBodySize, const std::string & _index,
                       const std::string & _name, const std::string & _root, const std::string & _host,
                       const std::string & _defaultErrorPage)
    : stop(false), serverConfig(static_cast<uint16_t>(_port), _host, _name, _root, _index,
                   _clientMaxBodySize, _defaultErrorPage, _host) {}

void ServerInfo::InitInfo()
{
	InitEpoll();
	InitSocket();
}
