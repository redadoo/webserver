#pragma once
#ifndef NETWORKUTILS_HPP
# define NETWORKUTILS_HPP

#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>

namespace NetworkUtils
{
    /// @brief Validates an IPv4 address.
    /// @param ipAddress The IP address string to validate.
    /// @return True if the IP address is valid, false otherwise.
    bool IsValidateIp(const std::string &ipAddress);

    /// @brief Converts an IPv4 address from binary to text form.
    /// @param addr A pointer to the sockaddr_in structure containing the IPv4 address.
    /// @param src_ip_buf A buffer to store the converted IP address.
    /// @return A pointer to the converted IP address string.
    const char *ConvertAddrNtop(struct sockaddr_in *addr, char *src_ip_buf);

    /// @brief Checks if a string is a valid domain name.
    /// @param str The string to check.
    /// @return True if the string is a valid domain name, false otherwise.
    bool IsDomain(const std::string& str);

	bool SetNonBlocking(int fd);

} // namespace NetworkUtils

#endif
