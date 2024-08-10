#include "utils.hpp"

bool utils::CheckFileExistence(const char *fileName)
{
	std::ifstream file(fileName);
	return file.good();
}

std::string utils::ToString(int n)
{
	std::stringstream ss;      
	ss << n;                 
	return ss.str();
}

std::string utils::ToString(uint16_t n)
{
	std::stringstream ss;      
	ss << n;                 
	return ss.str();
}

const char *utils::ConvertAddrNtop(sockaddr_in *addr, char *src_ip_buf)
{
	const char *ret;
	in_addr_t saddr = addr->sin_addr.s_addr;

	ret = inet_ntop(AF_INET, &saddr, src_ip_buf, sizeof("xxx.xxx.xxx.xxx"));
	if (ret == NULL)
		throw WebServerException::ExceptionErrno("inet_ntop(): ", errno);

	return ret;
}
