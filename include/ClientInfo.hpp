
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
	uint16_t            my_index;

    ClientInfo(bool used, int fd, const std::string& ip, uint16_t port, uint16_t index)
        : is_used(used),           
          client_fd(fd),           
          src_ip(ip),              
          src_port(port),          
          my_index(index) {}
};


#endif