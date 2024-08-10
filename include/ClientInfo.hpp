
#ifndef CLIENTINFO_HPP
# define CLIENTINFO_HPP

# include <stdint.h>
# include <string>

struct ClientInfo
{
	bool				is_used;
	int 				client_fd;
	std::string         src_ip;
	uint16_t            src_port;

    ClientInfo() 
        : is_used(false), client_fd(-1), src_ip(""), src_port(0) {}

	ClientInfo(bool used, int fd, const std::string& ip, uint16_t port)
			: is_used(used),           
				client_fd(fd),           
				src_ip(ip),              
				src_port(port) {}
};


#endif