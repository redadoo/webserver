#ifndef SERVERINFO_HPP
# define SERVERINFO_HPP

# include <ClientInfo.hpp>
# include <sys/socket.h>
# include <sys/epoll.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netinet/tcp.h>
# include <map>
# include <EpollUtils.hpp>
# include <WebServerException.hpp>
# include <utils.hpp>


struct ServerConfig
{
	const uint16_t								serverPort;
	const std::string							host;
	const std::string							serverName;
	const std::string							serverRoot;
	const std::string 							indexPage;
	const std::string							clientMaxBody;
	const std::string 							errorPage;
	const std::string 							socketIp;

    ServerConfig(uint16_t port, const std::string& h, const std::string& name,
                 const std::string& root, const std::string& index,
                 const std::string& maxBody, const std::string& error,
                 const std::string& ip)
        : serverPort(port), host(h), serverName(name), serverRoot(root),
          indexPage(index), clientMaxBody(maxBody), errorPage(error), socketIp(ip) {}
};

class ServerInfo
{
private:	
	void InitSocket(int epollFd);
public:
	int								serverFd;
	struct sockaddr_in 				serverAddress;
	bool							stop;

	ServerConfig					serverConfig;
	std::map<int, ClientInfo>	clientsInfo;
	ServerInfo(int _port, const std::string &_clientMaxBodySize, const std::string & _index, 
				const std::string & _name, const std::string & _root, const std::string & _host, const std::string & _defaultErrorPage);
	
	void InitInfo(int epollFd);
};

#endif
