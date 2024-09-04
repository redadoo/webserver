#ifndef STRINGUTILS_HPP
# define STRINGUTILS_HPP

#include <string>
#include <vector>
#include <stdint.h>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <sys/types.h>

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

	std::string ToString(size_t n);

	std::string ToString(unsigned long long n);

	std::string ToString(off_t n);

    /// @brief Converts an string to integer.
    /// @param str The string to convert.
    /// @return A int representation of the string.
    int StrintToInt(const std::string& str);

	unsigned long long StringToUnsignedLongLong(const std::string& str);

	size_t StringToSizeT(const std::string& str);

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

	std::string FormatTime(time_t time);

	std::string Trim(const std::string& str);

    /// @brief Splits a given URL path into the base path and query string components.
    /// @param path The full URL path that may contain a query string.
    /// @return A pair where the first element is the base path and the second element is the query string.
    std::pair<std::string, std::string> SplitPathAndQuery(const std::string& path);

    /// @brief Extracts the file content from a multipart/form-data part, skipping over headers and whitespace.
    /// @param part The multipart data segment containing headers and file content.
    /// @return A string containing the extracted file content. Returns an empty string if the content could not be located.
    std::string ExtractFileContent(const std::string& part);

    /// @brief Extracts the filename from a multipart/form-data part by searching for the "filename" attribute in the headers.
    /// @param part The multipart data segment containing headers and file content.
    /// @return A string containing the extracted filename. Returns an empty string if the filename could not be located.
    std::string ExtractFilename(const std::string& part);

    /// @brief Extracts the boundary string from the Content-Type header of a multipart/form-data request.
    /// @param contentType The Content-Type header value containing the boundary attribute.
    /// @return A string containing the extracted boundary. If no boundary is found, returns an empty string.
    std::string GetBoundary(const std::string& contentType);

    /// @brief Splits the multipart/form-data request body into individual parts using the provided boundary string.
    /// @param data The complete multipart/form-data content.
    /// @param boundary The boundary string used to delimit the different parts of the multipart data.
    /// @return A vector of strings, each containing an individual part of the multipart data. Empty parts are skipped.
    std::vector<std::string> SplitMultipartData(const std::string& data, const std::string& boundary);

    /// @brief Extracts the script path from a URL by removing any query string that follows the `?` character.
    /// @param path The full URL path which may include a query string.
    /// @return The script path without the query string.
    std::string GetScriptPath(const std::string& path);

} // namespace StringUtils

#endif
