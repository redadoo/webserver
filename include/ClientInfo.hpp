
#ifndef CLIENTINFO_HPP
# define CLIENTINFO_HPP

# include <stdint.h>
# include <string>

// enum ResponseType
// {

// }


/// @brief Structure to store information about a client
struct ClientInfo
{
	int 				clientFd;
	std::string			srcIp;
	uint16_t			srcPort;

	ClientInfo()
		: clientFd(-1), srcIp(""), srcPort(0) {}

	ClientInfo(int fd, const std::string& ip, uint16_t port)
			: 	clientFd(fd),
				srcIp(ip),
				srcPort(port) {}

	void ParseResponse(const char* response);
};


#endif