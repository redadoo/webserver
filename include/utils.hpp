#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <fstream>
# include <sstream>
# include <stdint.h>
# include "netinet/in.h"

namespace utils 
{
    /// @brief Checks if a file exists at the given path.
    /// @param fileName The path to the file to check.
    /// @return True if the file exists, false otherwise.
    bool CheckFileExistence(const char* fileName);

    /// @brief Converts a 16-bit unsigned integer to a string.
    /// @param n The number to convert.
    /// @return A string representation of the number.
    std::string ToString(uint16_t n);

    /// @brief Converts an integer to a string.
    /// @param n The number to convert.
    /// @return A string representation of the number.
    std::string ToString(int n);

    /// @brief Converts an IPv4 address from binary to text form.
    /// @param addr A pointer to the sockaddr_in structure containing the IPv4 address.
    /// @param src_ip_buf A buffer to store the converted IP address.
    /// @return A pointer to the converted IP address string.
    const char *ConvertAddrNtop(struct sockaddr_in *addr, char *src_ip_buf);
}

#endif