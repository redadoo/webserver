#pragma once
#ifndef Client_HPP
# define Client_HPP

#include <ClientConfig.hpp>
#include <HttpMessage.hpp>

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