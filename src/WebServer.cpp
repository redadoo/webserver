#include <WebServer.hpp>
#include <Parser.hpp>
#include <EpollUtils.hpp>
#include <WebServerException.hpp>
#include <HandleSignal.hpp>
#include <Logger.hpp>
#include <StringUtils.hpp>

// global variable

HandleSignal::SignalState signalState;

// constructor/Destructors

WebServer::WebServer() {}

WebServer::~WebServer()
{
	Logger::Log("Closing WebServer ...");
	CleanUpAll();
	Logger::Log("all resources released and Web Server closed successfully");
}

//public function

void WebServer::InitServer(const char *configFIle)
{
	Parser::ParseConfigFile(servers, configFIle);
	CheckServerPort();
	HandleSignal::SetupSignalHandler();
	epollFd = EpollUtils::EpollInit();

	for (size_t i = 0; i < servers.size(); i++)
		servers[i].InitSocket(epollFd);

	needToStop = false;
	Logger::Log("successfully init all servers data");
}

void WebServer::StartServer()
{
	Logger::Log("Entering event loop...");

	int	epollRet;
	while (!needToStop)
	{
		needToStop = signalState.signCaught;
		if (needToStop)
			continue ;

		epollRet = epoll_wait(epollFd, events, MAX_EVENTS, TIMEOUT);
		if (epollRet == 0)
		{
			Logger::Log(std::string("quante sono ;") + StringUtils::ToString(servers[0].clients.size()));
			Logger::Log(std::string("quante sono ;") + StringUtils::ToString(servers[1].clients.size()));
			Logger::Log(TIMEOUT_STRING);
			continue;
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

// private function

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

void WebServer::CheckSockets(int epollRet)
{
	int	fd;

	for(std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		for (int i = 0; i < epollRet; i++)
		{
			fd = events[i].data.fd;
			if (fd == it->serverFd)
			{
				if (it->AcceptClient(fd, epollFd) < 0)
					continue ;
			}
			else if (EpollUtils::EpollCheckEventError(events[i].events))
			{
				Logger::LogWarning("close connection for epoll event error");
				it->CloseClientConnection(fd);
				break ;
			}
			else if (it->IsMyClient(fd))
				HandleClientEvent(it->GetClient(fd), *it);
		}
	}
}

void WebServer::HandleClientEvent(Client &client, Server &server)
{
	try
	{
		server.ReadClientRequest(client);
		server.ProcessRequest(client, 0);
		server.SendResponse(client);
	}
	catch(const WebServerException::HttpStatusCodeException& e) {
		server.SendErrorResponse(client, e.code);
	}
	catch (const std::exception &e) {
		Logger::LogError("Unexpected exception in HandleClientEvent occurred: " + std::string(e.what()));
	}
	
	server.CloseClientConnection(client.clientFd);
}

void WebServer::CleanUpAll()
{
	for (size_t i = 0; i < servers.size(); ++i)
		close(servers[i].serverFd);

	if (epollFd != -1)
		close(epollFd);
}