#include <EpollUtils.hpp>
#include <WebServerException.hpp>
#include <HandleSignal.hpp>
#include <Logger.hpp>
#include <StringUtils.hpp>
#include <WebServer.hpp>

// global variable

HandleSignal::SignalState signalState;

// constructor/Destructors

WebServer::WebServer() {}

WebServer::~WebServer()
{
	CleanUpAll();
}

// private function

void WebServer::HandleClientEvent(Client &client, uint32_t events, Server &server)
{
	if (EpollUtils::EpollCheckEventError(events))
	{
		Logger::ClientLog(server, client, "has closed its connection");
		server.CloseClientConnection(client);
		return ;
	}

	try
	{
		server.ReadClientResponse(client);
		server.ProcessRequest(client, 0);
	}
	catch(const WebServerException::HttpStatusCodeException& e)
	{
		Logger::LogError("HTTP error occurred: " + StringUtils::ToString(e.code));
		server.SendErrorResponse(client, e.code);
	}
	catch (const std::exception &e)
	{
		Logger::LogError("Unexpected exception occurred: " + std::string(e.what()));
		server.CloseClientConnection(client);
	}
}

void WebServer::CheckSockets(int epollRet)
{
	int	fd;

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
				servers[y].CloseClientConnection(fd);
				break ;
			}
			else if (servers[y].IsMyClient(fd))
			{
				HandleClientEvent(servers[y].GetClient(fd), events[i].events, servers[y]);
			}
		}
	}
}

void WebServer::CheckServerPort()
{
	for (size_t i = 0; i < servers.size(); i++)
	{
		for (size_t j = 0; j < servers.size(); j++)
		{
			if (i == j)
				continue;

			if(servers[i].serverConfig.serverPort == servers[j].serverConfig.serverPort)
				throw std::invalid_argument("same port on different server");
		}
	}
}

void WebServer::CleanUpAll()
{
	for (size_t i = 0; i < servers.size(); ++i)
	{
		close(servers[i].serverFd);
	}

	if (epollFd != -1)
		close(epollFd);
}

//public function

void WebServer::InitServer(const char *configFIle)
{
	Parser::ParseConfigFile(servers, configFIle);

	HandleSignal::SetupSignalHandler();

	epollFd = EpollUtils::EpollInit();

	for (size_t i = 0; i < servers.size(); i++)
		servers[i].InitSocket(epollFd);

	Logger::Log("successfully init all servers data");
}

void WebServer::StartServer()
{
	int	epollRet;

	needToStop = false;
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


