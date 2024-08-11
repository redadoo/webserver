
#ifndef CLIENTINFO_HPP
# define CLIENTINFO_HPP

# include <stdint.h>
# include <string>

/// @brief Structure to store information about a client
struct ClientInfo
{
	int 				client_fd;
	std::string			src_ip;
	uint16_t			src_port;

    ClientInfo()
        : client_fd(-1), src_ip(""), src_port(0) {}

	ClientInfo(int fd, const std::string& ip, uint16_t port)
			: 	client_fd(fd),           
				src_ip(ip),              
				src_port(port) {}
};


#endif