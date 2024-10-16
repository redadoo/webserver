#include <NetworkUtils.hpp>
#include <WebServerException.hpp>
#include <Logger.hpp>
#include <fcntl.h>

const char *NetworkUtils::ConvertAddrNtop(sockaddr_in *addr, char *src_ip_buf)
{
	in_addr_t saddr = addr->sin_addr.s_addr;

	const char *ret = inet_ntop(AF_INET, &saddr, src_ip_buf, INET_ADDRSTRLEN);
	if (ret == NULL)
		throw WebServerException::ExceptionErrno("inet_ntop(): ", errno);

	return ret;
}


bool NetworkUtils::IsValidateIp(const std::string &ipAddress)
{
	struct sockaddr_in sa;
	int result = inet_pton(AF_INET, ipAddress.c_str(), &(sa.sin_addr));
	return result != 0;
}


bool NetworkUtils::IsDomain(const std::string& str)
{
	int numDots = 0;
	std::string label;

	if (str[0] == '-' || str[str.size() - 1] == '-' || str[0] == '.' || str[str.size() - 1] == '.')
		return false;

	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == '.')
		{
			if (label.size() == 0 || label.size() > 63)
				return false;
			if (label[0] == '-' || label[label.size() - 1] == '-')
				return false;
			label = "";
			numDots++;
		}
		else if (str[i] == '-')
		{
			if (label.size() == 0 || label.size() > 63)
				return false;
			label = "";
		}
		else
			label += str[i];
	}

	if (str.size() == 0 || str.size() > 253)
		return false;
	return true;
}

bool NetworkUtils::SetNonBlocking(const int fd)
{
	if (fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK) == -1)
	{
		Logger::LogError("Failed to set file descriptor flags" + std::string(strerror(errno)));
		return false;
	}

	Logger::Log("File descriptor set to non-blocking mode");
	return true;
}
