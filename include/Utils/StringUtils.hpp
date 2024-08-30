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

	std::string ToString(unsigned long long n);

    /// @brief Converts an string to integer.
    /// @param str The string to convert.
    /// @return A int representation of the string.
    int StrintToInt(const std::string& str);

	unsigned long long StringToUnsignedLongLong(const std::string& str);

    /// @brief Splits a string into substrings based on a delimiter.
    /// @param str The string to split.
    /// @param delim The delimiter character.
    /// @return A vector of substrings.
    std::vector<std::string> Split(const std::string& str, char delim);

    /// @brief Splits a string into substrings based on a delimiter.
    /// @param str The string to split.
    /// @param delim The delimiter string.
    /// @return A vector of substrings.
    std::vector<std::string> Split(const std::string& str, const std::string& delim);

    /// @brief Checks if a string contains only digit characters.
    /// @param str The string to check.
    /// @return True if all characters in the string are digits, false otherwise.
    bool IsAllDigit(const std::string& str);
} // namespace StringUtils

#endif
