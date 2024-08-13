
#ifndef Client_HPP
# define Client_HPP

# include <stdint.h>
# include <string>

/// @brief Structure to store information about a client
struct Client
{
	int 				clientFd;
	std::string			srcIp;
	uint16_t			srcPort;

	Client()
		: 
		clientFd(-1), 
		srcIp(""), 
		srcPort(0) {}


	Client(int fd, const std::string& ip, uint16_t port)
			: 	
			clientFd(fd),
			srcIp(ip),
			srcPort(port) {}

	void ParseResponse(const char* response);
};


#endif