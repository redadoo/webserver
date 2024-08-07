#include <WebServer.hpp>

void WebServer::StartServer()
{
	while (true) 
	{
        int numEvents = epoll_wait(serverInfo[0].epollFd, serverInfo[0].events, MAX_EVENTS, -1);
        if (numEvents == -1) 
		{
			Logger::LogError("Failed to wait for events.");
            break;
        }

        for (int i = 0; i < numEvents; ++i) 
		{
            if (serverInfo[0].events[i].data.fd == serverInfo[0].serverFd) 
			{
                // Accept new client connection
                struct sockaddr_in clientAddress;
                socklen_t clientAddressLength = sizeof(clientAddress);
                int clientFd = accept(serverInfo[0].serverFd, (struct sockaddr*)&clientAddress, &clientAddressLength);
                if (clientFd == -1) 
				{
					Logger::LogError("Failed to accept client connection.");
                    continue;
                }

                // Set non-blocking mode for the client socket
                int flags = fcntl(clientFd, F_GETFL, 0);
                fcntl(clientFd, F_SETFL, flags | O_NONBLOCK);

                // Add client socket to epoll
                serverInfo[0].event.events = EPOLLIN | EPOLLET; // Edge-triggered mode
                serverInfo[0].event.data.fd = clientFd;
                if (epoll_ctl(serverInfo[0].epollFd, EPOLL_CTL_ADD, clientFd, &serverInfo[0].event) == -1) {
                    Logger::LogError("Failed to add client socket to epoll instance.");
                    close(clientFd);
                    continue;
                }
            } 
			else 
			{
				// Handle client data
				int clientFd = serverInfo[0].events[i].data.fd;
				char buffer[1024];
				int bytesRead = read(clientFd, buffer, sizeof(buffer));

				if (bytesRead <= 0) 
				{
					if (bytesRead == 0 || (bytesRead == -1 && errno != EAGAIN)) 
					{
						// Connection closed or an error occurred
						close(clientFd);
					}
				} 
				else 
				{
					// Echo the received data back to the client
					write(clientFd, buffer, bytesRead);
				}
            }
        }
    }

    close(serverInfo[0].serverFd);
    close(serverInfo[0].epollFd);
}

void WebServer::InitServer() 
{
	serverInfo[0].InitInfo();
}

WebServer::WebServer()
{
	try
	{
		Logger::Log("start parsing Configuration file : config/default.conf");
		std::vector<Token> tokens = Lexer::GetToken(DEFAULT_CONFIG_FILE);
		Parser::TryParse(serverInfo, tokens);
		Logger::Log("finished parsing Configuration file");
		tokens.clear();
	}
	catch(const std::exception& e)
	{
		Logger::LogException(e);
	}
}

WebServer::WebServer(const char *fileConf)
{
	try
	{
		Logger::Log("start parsing Configuration file : config/default.conf");

		std::vector<Token> tokens = Lexer::GetToken(fileConf);
		Parser::TryParse(serverInfo, tokens);

		Logger::Log("finished parsing Configuration file");
	}
	catch(const std::exception& e)
	{
		Logger::LogException(e);
	}
}

WebServer::~WebServer() {}
