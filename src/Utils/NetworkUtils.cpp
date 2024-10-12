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
	if (str.empty() 
		|| str.length() > 253 
		|| str[0] == '-' 
		|| str[str.length() - 1] == '-' 
		|| str[0] == '.' 
		|| str[str.length() - 1] == '.')
		return false;

	int numDots = 0;
	std::string label;

	for (size_t i = 0; i < str.size(); i++)
	{
		char ch = str[i];
		if (ch == '.')
		{
			if (label.empty() || label.size() > 63 || label[0] == '-' || label[label.size() - 1] == '-')
				return false;

			numDots++;
			label = "";
		}
		else
			label += ch;
	}

	if (numDots == 0 || label.empty() || label.size() > 63)  // Final label check
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
