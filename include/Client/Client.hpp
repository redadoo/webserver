
#ifndef Client_HPP
# define Client_HPP

#include <stdint.h>
#include <string>
#include <vector>
#include <ClientConfig.hpp>
#include <HttpMessage.hpp>

/// @brief Structure to store information about a client
class Client
{
public:

	ClientConfig				clientConfig;
	HttpMessage					request;
	int 						clientFd;
	int							contentLenght;
	

	Client();
	Client(int fd, const std::string& ip, uint16_t port);
};

#endif