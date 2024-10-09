#include <HttpMessage.hpp>
#include <Logger.hpp>
#include <WebServerException.hpp>
#include <StringUtils.hpp>
#include <algorithm>
#include <sys/types.h>

// CaseInsensitiveCompare

bool CaseInsensitiveCompare::char_compare(char ac, char bc)
{
	return (std::tolower(static_cast<unsigned char>(ac)) < std::tolower(static_cast<unsigned char>(bc)));
}

bool CaseInsensitiveCompare::operator()(const std::string &a, const std::string &b) const
{
	return (std::lexicographical_compare(
		a.begin(), a.end(),
		b.begin(), b.end(),
		CaseInsensitiveCompare::char_compare));
}

// private function

void HttpMessage::ParseStartLine(const Ustring &str)
{
	std::istringstream lineStream(str.toString());
	lineStream >> startLine.httpMethod >> startLine.path >> startLine.httpVersion;

	if(startLine.httpMethod != "GET" && startLine.httpMethod != "POST" && startLine.httpMethod != "DELETE")
		throw WebServerException::HttpStatusCodeException(HttpStatusCode::NotImplemented);

	if(startLine.path.length() > 1024)
		throw WebServerException::HttpStatusCodeException(HttpStatusCode::URITooLong);

	if(startLine.path.find("../") != std::string::npos || startLine.path == ".." )
		throw WebServerException::HttpStatusCodeException(HttpStatusCode::BadRequest);

	if(startLine.httpVersion != "HTTP/1.1")
		throw WebServerException::HttpStatusCodeException(HttpStatusCode::NotImplemented);

	if (startLine.httpMethod == "GET" || startLine.httpMethod == "DELETE")
		return;

}

// public function

void HttpMessage::ParseHeaders(Ustring& chunk)
{
	size_t start = 0;
	size_t end = chunk.find("\r\n");

	if (end == std::string::npos) 
		throw std::invalid_argument(" Invalid header format");

	// Parse request line (first line)
	Ustring request_line = chunk.substr(start, end - start);

	ParseStartLine(request_line);

	start = end + 2; // Skip "\r\n"

	while ((end = chunk.find("\r\n", start)) != std::string::npos) 
	{
		Ustring header_line = chunk.substr(start, end - start);

		size_t delimiter = header_line.find(":");

        if (delimiter == std::string::npos) 
			throw std::invalid_argument(" Invalid header line (no colon) ");

        std::string header_name = header_line.substr(0, delimiter + 2).toString();
        std::string header_value = header_line.substr(delimiter + 2).toString(); // Skip ": "
        header[header_name] = header_value;
		start = end + 2; // Skip "\r\n"
	}
}

void HttpMessage::ParseMessage(Ustring& chunk)
{
	if (!isHeaderComplete) 
	{
		chunk.find("\r\n\r\n");
		size_t header_end = chunk.find("\r\n\r\n");
		if (header_end != std::string::npos) 
		{
			// Headers and part of body in this chunk
			Ustring header_part = chunk.substr(0,header_end);
			Ustring tmp = chunk.substr(header_end + 4);
			
			if (tmp[0] != 0) 
			{
				body = tmp;
			}
			
			ParseHeaders(header_part); 
			isHeaderComplete = true;
		} 
		else 
		{
			incomplete_header_buffer += chunk.toString();
		}
	} 
	else
	{
		body.content += chunk;
	}
}

std::string HttpMessage::ToString() const
{
	std::string msg = "";
	msg.append(startLine.ToString());
	for (Header::const_iterator it = header.begin(); it != header.end(); ++it)
	{
		msg.append(it->first);
		msg.append(it->second);
		msg.append("\n");
	}
	msg.append(body.content.toString());
	return msg;
}

unsigned long long HttpMessage::GetContentLength() const
{
	long long res;
	std::string value;

	try	 
	{
		value = header.at("Content-Length: ");
	}
	catch(const std::exception& e) 	
	{
		value = "0";
	}
	res = StringUtils::StringToLongLong(value);
	if(res < 0)
		return 0;

	return res;
}

bool HttpMessage::IsMessageComplete(const unsigned long long maxBodySize, ssize_t recvRet) const
{
	(void)recvRet;
	unsigned long long contentLength = GetContentLength();

	if (body.size() > maxBodySize)
		throw WebServerException::HttpStatusCodeException(HttpStatusCode::PayloadTooLarge);
	
	if (contentLength == 0)
		return header.size() > 0;

	return body.size() >= contentLength;
}

long long HttpMessage::size() const
{
	long long headerSize = 0;

	if (header.size() > 0)
	{
		for (Header::const_iterator it = header.begin(); it != header.end(); ++it)
			headerSize += it->first.size() + it->second.size();
	}
	
	if (startLine.size() > 0)
		return (startLine.size() + headerSize + body.size());
	
	return (startLine.size() + headerSize + body.size());	
}

std::ostream &operator<<(std::ostream &os, const HttpMessage &msg)
{
	os << "[Start Header]\n";
	os << msg.startLine.ToString();
	for (Header::const_iterator it = msg.header.begin(); it != msg.header.end(); ++it)
		os << it->first << it->second << "\n";
	os << "[End Header]\n";

	if (msg.body.size() > 0)
	 	os << "[Start Body]\n" << msg.body << "\n" << "[End Body]\n";
		
	return (os);
}

StartLine::StartLine()
{
	this->httpMethod = "";
	this->path = "";
	this->httpVersion = "";
	this->isInit = false;
}

std::string StartLine::ToString() const
{
	std::string startLine;

	startLine.append(this->httpMethod);
	startLine.append(" ");
	startLine.append(this->path);
	startLine.append(" ");
	startLine.append(this->httpVersion);
	startLine.append("\n");

	return startLine;
}
