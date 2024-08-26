#ifndef STRINGUTILS_HPP
# define STRINGUTILS_HPP

#include <string>
#include <vector>
#include <stdint.h>

namespace StringUtils 
{

    /// @brief Converts a 16-bit unsigned integer to a string.
    /// @param n The number to convert.
    /// @return A string representation of the number.
    std::string ToString(uint16_t n);

    /// @brief Converts an integer to a string.
    /// @param n The number to convert.
    /// @return A string representation of the number.
    std::string ToString(int n);

    /// @brief Converts an string to integer.
    /// @param str The string to convert.
    /// @return A int representation of the string.
    int StrintToInt(const std::string& str);

    std::vector<std::string> Split(const std::string& str, char delim);

    bool IsAllDigit(const std::string& str);
} // namespace StringUtils

#endif