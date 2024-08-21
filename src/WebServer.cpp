# include <EpollUtils.hpp>
# include <Logger.hpp>
# include <WebServer.hpp>
# include <WebServerSignal.hpp>
# include <WebServerException.hpp>
# include <StringUtils.hpp>

WebServerSignal::SignalState signalState;

WebServer::WebServer() {}

WebServer::~WebServer() {
	CleanUpAll();
}

void WebServer::InitServer(const char *configFIle)
{
	Parser::ParseConfigFile(servers, configFIle);

	WebServerSignal::SetupSignalHandler();

	epollFd = EpollUtils::EpollInit();

	for (size_t i = 0; i < servers.size(); i++)
		servers[i].Init(epollFd);

	Logger::Log("successfully init all servers data");
}

void WebServer::StartServer()
{
	int epollRet;

	Logger::Log("Entering event loop...");

	needToStop = false;

	while (!needToStop)
	{
		needToStop = signalState.signCaught;
		if (needToStop)
			continue ;

		epollRet = epoll_wait(epollFd, events, MAX_EVENTS, TIMEOUT);

		if (epollRet == 0)
		{
			Logger::Log(std::string("I don't see any event within ")
					+ StringUtils::ToString(TIMEOUT) + " milliseconds");
			continue ;
		}

		if (epollRet == -1)
		{
			if (errno == EINTR)
			{
				Logger::LogError("Something interrupted me!");
				continue ;
			}
			else
				throw WebServerException::ExceptionErrno("epoll_wait(): ", errno);
		}

		CheckSockets(epollRet);
	}
}

void WebServer::CheckSockets(int epollRet)
{
	int fd;

	for (int y = 0; y < (int)servers.size(); y++)
	{
		for (int i = 0; i < epollRet; i++)
		{
			fd = events[i].data.fd;

			if (fd == servers[y].serverFd)
			{
				if (servers[y].AcceptClient(fd, epollFd) < 0)
					continue ;
			}
			else if (EpollUtils::EpollCheckEventError(events[i].events))
			{
				Logger::Log("test");
				servers[y].CloseClientConnection(fd);
				break;
			}
			else if (servers[y].IsMyClient(fd))
			{
				HandleClientEvent(servers[y].GetClient(fd), events[i].events, servers[y]);
			}
		}
	}
}

void WebServer::HandleClientEvent(Client &client, uint32_t events, Server &server)
{
	if (EpollUtils::EpollCheckEventError(events))
	{
		Logger::ClientLog(server, client, "has closed its connection");
		server.CloseClientConnection(client);
		return;
	}

	server.ReadClientResponse(client);
	server.ParseClientResponse(client);
	server.SendResponse(client);
}

void WebServer::CleanUpAll()
{
    for (size_t i = 0; i < servers.size(); ++i)
	{
		for (size_t y = 0; y < servers[i].clients.size(); y++)
		{
			servers[i].CloseClientConnection(servers[i].clients[y]);
		}

		close(servers[i].serverFd);
    }

	if (epollFd != -1)
    	close(epollFd);
}
