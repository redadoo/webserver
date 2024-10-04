#ifndef HTTPMESSAGE_HPP
# define HTTPMESSAGE_HPP

#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdint.h>

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

/// @brief Functor for case-insensitive comparison of strings
struct CaseInsensitiveCompare {
	bool operator()(const std::string& a, const std::string& b) const;
	static bool char_compare(char ac, char bc);
};

/// @brief Header map with case-insensitive key comparison
typedef std::map<std::string, std::string, CaseInsensitiveCompare> Header;

// Class for managing HTTP messages
class HttpMessage
{
private:
    std::string incomplete_header_buffer;

public:
	StartLine 	startLine;
	Header      header;
	std::string body;
    bool        isHeaderComplete;

    void ParseHeaders(std::string &chunk);

	/// @brief Parses the HTTP message from a string chunk.
    /// @param messageChunk The chunk of the HTTP message to parse.
    void ParseMessage(std::string &chunk);

    /// @brief Returns the size of the HTTP message.
    /// @return Size of the HTTP message.
    long long size() const;

    /// @brief Provides a C-style string representation of the HTTP message.
    /// @return A C-style string of the HTTP message.
    std::string ToString() const;

    unsigned long long GetContentLength() const;

    bool IsMessageComplete() const;

    /// @brief Provides a string representation of the HTTP message.
    /// @param os Output stream to write the message to.
    /// @param msg HTTP message object to be written.
    /// @return The output stream with the HTTP message written.
    friend std::ostream& operator<<(std::ostream& os, const HttpMessage& msg);

private:

   	/// @brief Parses the start line of the HTTP message.
    /// @param str The start line string to parse.
    void ParseStartLine(const std::string& str);
};

#endif
