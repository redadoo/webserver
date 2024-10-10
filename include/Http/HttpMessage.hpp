#pragma once
#ifndef HTTPMESSAGE_HPP
# define HTTPMESSAGE_HPP

#include <Body.hpp>
#include <Header.hpp>
#include <StartLine.hpp>
#include <sys/types.h>
// Class for managing HTTP messages
class HttpMessage
{
private:
    std::string incomplete_header_buffer;

   	/// @brief Parses the start line of the HTTP message.
    /// @param str The start line string to parse.
    void ParseStartLine(const Ustring& str);

public:
	StartLine 	startLine;
	Header      header;
    Body        body;
    bool        isBodyBinary;
    bool        isHeaderComplete;

    void ParseHeaders(Ustring& chunk);

	/// @brief Parses the HTTP message from a string chunk.
    /// @param messageChunk The chunk of the HTTP message to parse.
    void ParseMessage(Ustring& chunk);

    /// @brief Returns the size of the HTTP message.
    /// @return Size of the HTTP message.
    long long size() const;

    /// @brief Provides a C-style string representation of the HTTP message.
    /// @return A C-style string of the HTTP message.
    std::string ToString() const;

    unsigned long long GetContentLength() const;

    bool IsMessageComplete(const unsigned long long maxBodySize,ssize_t recvRet) const;

    /// @brief Provides a string representation of the HTTP message.
    /// @param os Output stream to write the message to.
    /// @param msg HTTP message object to be written.
    /// @return The output stream with the HTTP message written.
    friend std::ostream& operator<<(std::ostream& os, const HttpMessage& msg);
};

#endif
