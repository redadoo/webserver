#ifndef StartLine_HPP
# define StartLine_HPP

#include <string>

/// @brief Structure for the start line of an HTTP message
struct StartLine
{
	std::string					httpMethod;
	std::string					httpVersion;
	std::string					path;
	bool 						isInit;

    StartLine();
	size_t size() const {return httpMethod.size() + httpVersion.size() + path.size();}
    std::string ToString() const;
};

#endif