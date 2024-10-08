#ifndef STRINGUTILS_HPP
# define STRINGUTILS_HPP

#include <string>
#include <vector>
#include <stdint.h>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <sys/types.h>
#include <map>
#include <Body.hpp>

namespace StringUtils
{
	/// @brief Converts an integer to a string.
	/// @param n The number to convert.
	/// @return A string representation of the number.
	std::string ToString(int n);

	/// @brief Converts a 16-bit unsigned integer to a string.
	/// @param n The number to convert.
	/// @return A string representation of the number.
	std::string ToString(uint16_t n);
	
	/// @brief Converts an size_t to a string.
	/// @param n The number to convert.
	/// @return A string representation of the number.
	std::string ToString(size_t n);

	/// @brief Converts an unsigned long long to a string.
	/// @param n The number to convert.
	/// @return A string representation of the number.
	std::string ToString(unsigned long long n);

	/// @brief Converts an long long to a string.
	/// @param n The number to convert.
	/// @return A string representation of the number.
	std::string ToString(long long n);

	/// @brief Converts an off_t to a string.
	/// @param n The number to convert.
	/// @return A string representation of the number.
	std::string ToString(off_t n);

	/// @brief Converts an string to integer.
	/// @param str The string to convert.
	/// @return A int representation of the string.
	int StrintToInt(const std::string& str);

	/// @brief Converts an string to unsigned long long.
	/// @param str The string to convert.
	/// @return A unsigned long long representation of the string.
	unsigned long long StringToUnsignedLongLong(const std::string& str);

	/// @brief Converts an string to size_t.
	/// @param str The string to convert.
	/// @return A size_t representation of the string.
	size_t StringTouint16_t(const std::string& str);

	/// @brief Converts an string to long long.
	/// @param str The string to convert.
	/// @return A long long representation of the string.
	long long StringToLongLong(const std::string& str);
	
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

	/// @brief Formats a `time_t` value into a human-readable string representation of the date and time.
	/// @param time The `time_t` value representing the time to be formatted.
	/// @return A string containing the formatted date and time in the format "dd-Mon-yyyy HH:mm". Returns "Unknown time" if the time could not be converted.
	std::string FormatTime(time_t time);

	/// @brief Removes leading and trailing whitespace characters (spaces, tabs, newlines, and carriage returns) from a string.
	/// @param str The input string to be trimmed.
	/// @return A string with leading and trailing whitespace removed. Returns an empty string if the input string contains only whitespace.
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
	std::vector<Body> SplitMultipartData(const Body& body, const std::string& boundary);

	/// @brief Extracts the script path from a URL by removing any query string that follows the `?` character.
	/// @param path The full URL path which may include a query string.
	/// @return The script path without the query string.
	std::string GetScriptPath(const std::string& path);

	/// @brief Converts a map of key-value pairs to a null-terminated array of "key=value" C-strings.
	/// @param map The map of key-value pairs.
	/// @return A dynamically allocated array of C-strings. Caller must free the memory.
	char **GetMatrix(const std::map<std::string,std::string>& map);

} // namespace StringUtils

#endif
