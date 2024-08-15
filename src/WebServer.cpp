# include <EpollUtils.hpp>
# include <Logger.hpp>
# include <WebServer.hpp>
# include <WebServerSignal.hpp>
# include <utils.hpp>
# include <WebServerException.hpp>

WebServerSignal::SignalState signalState;

WebServer::WebServer() {
	try {
		Parser::FillServer(servers, DEFAULT_CONFIG_FILE);
	} catch (const std::exception &e) {
		Logger::LogException(e);
	}
}

WebServer::WebServer(const char *filePath) {
	try {
		Parser::FillServer(servers, filePath);
	} catch (const std::exception &e) {
		Logger::LogException(e);
	}
}

WebServer::~WebServer() {
	CleanUpAll();
}

void WebServer::InitServer()
{
	this->epollFd = -1;
	needToStop = false;
	epollFd = EpollUtils::EpollInit();
	
	WebServerSignal::SetupSignalHandler();
	Logger::Log("handled the signals successfully");

	for (size_t i = 0; i < servers.size(); i++)
		servers[i].Init(epollFd);
		
	Logger::Log("successfully init all servers data");
}

void WebServer::StartServer()
{
	int epollRet;

	Logger::Log("Entering event loop...");

	while (!needToStop)
	{
		needToStop = signalState.signCaught;
		if (needToStop)
			continue ;

		epollRet = epoll_wait(epollFd, events, MAX_EVENTS, TIMEOUT);

		if (epollRet == 0)
		{
			Logger::Log(std::string("I don't see any event within ")
					+ utils::ToString(TIMEOUT) + " milliseconds");
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
				servers[y].CloseClientConnection(servers[y].GetClient(fd),epollFd);
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
		server.CloseClientConnection(client, epollFd);
		return;
	}
	
	server.ReadClientResponse(client, epollFd);
	server.ParseClientResponse(client, epollFd);
}

void WebServer::CleanUpAll() 
{
    for (size_t i = 0; i < servers.size(); ++i) 
	{
        for (std::map<int, Client>::iterator it = servers[i].clients.begin(); it != servers[i].clients.end(); ++it) 
		{
			if (it->second.clientFd != -1)
			{
            	close(it->second.clientFd);
				EpollUtils::EpollDelete(epollFd, it->second.clientFd);
			}

        }
		if (servers[i].serverFd != -1)
		{
			EpollUtils::EpollDelete(epollFd, servers[i].serverFd);
        	close(servers[i].serverFd);
		}
    }
	if (epollFd != -1)
    	close(epollFd);
}