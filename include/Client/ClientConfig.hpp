#ifndef CLIENTCONFIG_HPP
# define CLIENTCONFIG_HPP

#include <stdint.h>
#include <string>

struct ClientConfig
{
    std::string					srcIp;
	uint16_t					srcPort;
};

#endif