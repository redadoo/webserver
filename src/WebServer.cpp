# include <WebServer.hpp>
# include <utils.hpp>
# include <EpollUtils.hpp>

WebServerSignal::SignalState signalState;

int WebServer::AcceptClient(int tcp_fd, const ServerInfo& serverInfo)
{
	int err;
    int client_fd;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    uint16_t src_port;
    const char* src_ip;
    char src_ip_buf[sizeof("xxx.xxx.xxx.xxx")];

    memset(&addr, 0, sizeof(addr));
    client_fd = accept(tcp_fd, (struct sockaddr*)&addr, &addr_len);
    if (client_fd < 0) {
        err = errno;
        if (err == EAGAIN)
            return 0;

		Logger::LogWarning("error on accept(): ");
        return -1;
    }
    
	src_port = ntohs(addr.sin_port);
    src_ip = utils::ConvertAddrNtop(&addr, src_ip_buf);
    if (!src_ip) 
	{
		Logger::LogWarning("Cannot parse source address");
        close(client_fd);
        return -1;
    }

	if(serverInfo.clientsInfo.find(client_fd) != serverInfo.clientsInfo.end())
	{
		Logger::Log("Client alredy connected");
	}
	else
	{
		ClientInfo client(true, client_fd, src_ip, src_port, 0);
		std::pair<int, ClientInfo> client_pair(client_fd, client);
		// serverInfo.clientsInfo.insert(client_pair);
		EpollUtils::EpollAdd(epollFd, client_fd, EPOLLIN | EPOLLPRI);
	}
	
	Logger::ClientLog(serverInfo, src_ip, src_port, " has been accepted!");
	return 0;
}

void WebServer::CheckSockets()
{
	for (int i = 0; i < epoll_ret; i++)
	{
		fd = events[i].data.fd;

		for (int y = 0; y < (int)serverInfos.size(); y++)
		{
			if (fd == serverInfos[y].serverFd)
			{
				Logger::Log("A client is trying to connecting to us ");
				if (AcceptClient(fd, serverInfos[y]) < 0)
					continue ;
			}

			HandleClientEvent(fd, events[i].events, serverInfos[y]);
		}
		
	}
}

void WebServer::HandleClientEvent(int client_fd, uint32_t revents, const ServerInfo& serverInfo)
{
	const uint32_t	err_mask = EPOLLERR | EPOLLHUP;
	char			buffer[1024];
	ssize_t			recv_ret;

	if (serverInfo.clientsInfo.find(client_fd) == serverInfo.clientsInfo.end())
		return;

	ClientInfo client = serverInfo.clientsInfo.find(client_fd)->second;

	if (revents & err_mask)
	{
		Logger::ClientLog(serverInfo, client.src_ip, client.src_port,
			"has closed its connection");
		return (CloseConnection(client, serverInfo));
	}
	recv_ret = recv(client_fd, buffer, sizeof(buffer), 0);
	if (recv_ret == 0)
		return CloseConnection(client, serverInfo);
	if (recv_ret < 0)
	{
		if (errno == EAGAIN)
			return ;
		WebServerException::ExceptionErrno("recv(): ", errno);
		return (CloseConnection(client, serverInfo));
	}
	
	buffer[recv_ret] = '\0';
	if (buffer[recv_ret - 1] == '\n')
		buffer[recv_ret - 1] = '\0';

	Logger::ClientLog(serverInfo, client.src_ip, client.src_port, buffer);
}

void WebServer::CloseConnection(ClientInfo client, const ServerInfo& serverInfo) 
{
	Logger::ClientLog(serverInfo, client.src_ip, client.src_port, "has been deleted ");
	EpollUtils::EpollDelete(epollFd,client.client_fd);
	close(client.client_fd);
}

void WebServer::StartServer()
{
	Logger::Log("Entering event loop...");

	while (!needToStop)
	{
		needToStop = signalState.signCaught;

		if(needToStop) 
			continue;

		epoll_ret = epoll_wait(epollFd, events, maxevents, timeout);

		if (epoll_ret == 0)
		{
			Logger::Log(std::string("I don't see any event within ")
					+ utils::ToString(timeout) + " milliseconds");
			continue ;
		}

		if (epoll_ret == -1)
		{
			err = errno;
			if (err == EINTR)
			{
				Logger::LogError("Something interrupted me!");
				continue ;
			}
			else
				throw WebServerException::ExceptionErrno("epoll_wait(): ", err);
		}

		CheckSockets();
	}
}

void WebServer::InitServer()
{
	timeout = 3000;
	maxevents = 32;
	needToStop = false;

	epollFd = EpollUtils::EpollInit();
	
	try
	{
		WebServerSignal::SetupSignalHandler();
		
		Logger::Log("handled the signals successfully");
		
		for (size_t i = 0; i < serverInfos.size(); i++)
			serverInfos[i].InitInfo(epollFd);

		Logger::Log("successfully init all servers data");
	}
	catch (const std::exception &e)
	{
		Logger::LogException(e);
	}

}

WebServer::WebServer()
{	
	try {
		Parser::FillServerInfo(serverInfos, DEFAULT_CONFIG_FILE);
	}
	catch (const std::exception &e) {
		Logger::LogException(e);
	}
}

WebServer::WebServer(const char *fileConf)
{
	try {
		Parser::FillServerInfo(serverInfos, fileConf);
	}
	catch (const std::exception &e) {
		Logger::LogException(e);
	}
}

WebServer::~WebServer() {}
