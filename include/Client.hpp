
#ifndef Client_HPP
# define Client_HPP

# include <stdint.h>
# include <string>
# include <vector>
# include <ClientConfig.hpp>

/// @brief Structure to store information about a client
class Client
{
public:

	int 						clientFd;
	std::vector<std::string>	lastResponse;
	ClientConfig				clientConfig;
	std::string					httpMethod;
	std::string					httpVersion;
	std::string					path;
	std::string					contentType;
	int							contentLenght;
	

	Client();
	Client(int fd, const std::string& ip, uint16_t port);

	void InitClientValue(const char* response);
};


#endif