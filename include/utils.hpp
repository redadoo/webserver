#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <fstream>
# include <sstream>
# include <stdint.h>
# include "netinet/in.h"
# include <arpa/inet.h>
# include <WebServerException.hpp>
namespace utils 
{
    bool CheckFileExistence(const char* fileName);
    std::string ToString(uint16_t n);
    std::string ToString(int n);

    const char *ConvertAddrNtop(struct sockaddr_in *addr, char *src_ip_buf);
}

#endif