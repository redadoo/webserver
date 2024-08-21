#ifndef NETWORKUTILS_HPP
# define NETWORKUTILS_HPP

namespace NetworkUtils
{
    bool validateIpAddress(const std::string &ipAddress);

    /// @brief Converts an IPv4 address from binary to text form.
    /// @param addr A pointer to the sockaddr_in structure containing the IPv4 address.
    /// @param src_ip_buf A buffer to store the converted IP address.
    /// @return A pointer to the converted IP address string.
    const char *ConvertAddrNtop(struct sockaddr_in *addr, char *src_ip_buf);
    
    bool IsDomain(const std::string& str);
    
} // namespace NetworkUtils

#endif