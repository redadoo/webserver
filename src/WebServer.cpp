#include <WebServer.hpp>
#include "unistd.h"

WebServerSignal::SignalState signalState;

int WebServer::AcceptClient(int tcp_fd)
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

        // Error handling
        printf("accept(): %s\n", strerror(err));
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

	if(serverInfo[0].clientsInfo.find(client_fd) != serverInfo[0].clientsInfo.end())
	{
		Logger::Log("Client alredy connected");
	}
	else
	{
		ClientInfo client(true, client_fd, src_ip, src_port, 0);
		std::pair<uint32_t, ClientInfo> client_pair(client_fd, client);
		serverInfo[0].clientsInfo.insert(client_pair);
		EpollUtils::EpollAdd(serverInfo[0].epollFd, client_fd, EPOLLIN | EPOLLPRI);
	}
	
	// Tell epoll to monitor this client file descriptor

	Logger::ClientLog(src_ip, src_port, "has been accepted!");
	return 0;
}

void WebServer::HandleClientEvent(int client_fd, uint32_t revents)
{
	const uint32_t	err_mask = EPOLLERR | EPOLLHUP;
	char			buffer[1024];
	ssize_t			recv_ret;

	if (serverInfo[0].clientsInfo.find(client_fd) == serverInfo[0].clientsInfo.end())
		return;

	ClientInfo client = serverInfo[0].clientsInfo.find(client_fd)->second;

	if (revents & err_mask)
	{
		Logger::ClientLog(client.src_ip, client.src_port,
			"has closed its connection");
		return (CloseConnection(client));
	}
	recv_ret = recv(client_fd, buffer, sizeof(buffer), 0);
	if (recv_ret == 0)
		return (CloseConnection(client));
	if (recv_ret < 0)
	{
		if (errno == EAGAIN)
			return ;
		WebServerException::ExceptionErrno("recv(): ", errno);
		return (CloseConnection(client));
	}
	
	buffer[recv_ret] = '\0';
	if (buffer[recv_ret - 1] == '\n')
		buffer[recv_ret - 1] = '\0';

	Logger::ClientLog(client.src_ip, client.src_port, buffer);
}

void WebServer::CloseConnection(ClientInfo client) 
{
	EpollUtils::EpollDelete(serverInfo[0].epollFd,client.client_fd);
	close(client.client_fd);
}

void WebServer::StartServer()
{
	int					err;
	int					maxevents;
	int					epoll_ret;
	int					epoll_fd;
	struct epoll_event	events[32];
	int					fd;

	int timeout = 3000;
	maxevents = 32;
	epoll_fd = serverInfo[0].epollFd;
	Logger::Log("Entering event loop...");
	while (!this->needToStop)
	{
		this->needToStop = signalState.signCaught;

		if(this->needToStop) 
			continue;

		epoll_ret = epoll_wait(epoll_fd, events, maxevents, timeout);
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
			{
				WebServerException::ExceptionErrno("epoll_wait(): ", err);
				break ;
			}
		}
		for (int i = 0; i < epoll_ret; i++)
		{
			fd = events[i].data.fd;
			if (fd == serverInfo[0].serverFd)
			{
				Logger::Log("A client is connecting to us ");
				if (AcceptClient(fd) < 0)
					continue ;
			}
			HandleClientEvent(fd, events[i].events);
		}
	}
}

void WebServer::InitServer()
{
	try
	{
		WebServerSignal::SetupSignalHandler();
		Logger::Log("handled the signals successfully");
		
		for (size_t i = 0; i < serverInfo.size(); i++)
			serverInfo[i].InitInfo();
		Logger::Log("successfully init all servers data");
	}
	catch (const std::exception &e)
	{
		Logger::LogException(e);
	}
}

WebServer::WebServer()
{
	this->needToStop = false;
	try
	{
		Parser::FillServerInfo(serverInfo, DEFAULT_CONFIG_FILE);
	}
	catch (const std::exception &e)
	{
		Logger::LogException(e);
	}
}

WebServer::WebServer(const char *fileConf)
{
	this->needToStop = false;
	try
	{
		Parser::FillServerInfo(serverInfo, fileConf);
	}
	catch (const std::exception &e)
	{
		Logger::LogException(e);
	}
}

WebServer::~WebServer() {}
