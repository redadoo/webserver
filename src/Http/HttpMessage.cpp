#include <HttpMessage.hpp>
#include <Logger.hpp>
#include <WebServerException.hpp>
#include <StringUtils.hpp>
#include <algorithm>
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

void HttpMessage::ParseStartLine(const std::string &str)
{
	std::istringstream lineStream(str);
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

void HttpMessage::ParseHeaders(std::string &header_part)
{
	size_t start = 0;
	size_t end = header_part.find("\r\n");
	Logger::LogWarning("find attiva windows");

	if (end == std::string::npos) 
		throw std::invalid_argument(" Invalid header format");

	// Parse request line (first line)
	std::string request_line = header_part.substr(start, end - start);
	Logger::LogWarning("find sub dio porco attiva windows");

	Logger::LogWarning("inizio find parse start line");
	ParseStartLine(request_line);
	Logger::LogWarning("fine find parse start line");

	// Parse headers
	Logger::LogWarning("inizio add cose ");
	start = end + 2; // Skip "\r\n"
	Logger::LogWarning("fine add cose ");

	while ((end = header_part.find("\r\n", start)) != std::string::npos) 
	{
		Logger::LogWarning("wedy");
		std::string header_line = header_part.substr(start, end - start);
		Logger::LogWarning("wedy sub");

		size_t delimiter = header_line.find(':');
		Logger::LogWarning("sono stanco");

        if (delimiter == std::string::npos) 
			throw std::invalid_argument(" Invalid header line (no colon) ");

        std::string header_name = header_line.substr(0, delimiter + 2);
		Logger::LogWarning("sono stanco sub2");
        std::string header_value = header_line.substr(delimiter + 2); // Skip ": "
		Logger::LogWarning("sono stanco sub3");
        header[header_name] = header_value;
		Logger::LogWarning("aggiungere cose basa");
		start = end + 2; // Skip "\r\n"
		Logger::LogWarning("finito aggiungere cose basa");
	}
}

void HttpMessage::ParseMessage(std::string &chunk)
{
	if (!isHeaderComplete) 
	{
		size_t header_end = chunk.find("\r\n\r\n");
		if (header_end != std::string::npos) 
		{
			// Headers and part of body in this chunk
			std::string header_part = chunk.substr(0, header_end);
			std::string tmp = chunk.substr(header_end + 4);
			
			if (tmp[0] != 0) 
				body = tmp;
			
			ParseHeaders(header_part); 
			isHeaderComplete = true;
		} 
		else 
		{
			incomplete_header_buffer += chunk;
		}
	} 
	else
	{
		body += chunk;
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
	msg.append(body);
	return msg;
}

unsigned long long HttpMessage::GetContentLength() const
{
	long long res;
	Logger::LogWarning("sono etrnato");
	std::string value;

	try	 {
		Logger::LogWarning("controllo value");
		value = header.at("Content-Length: ");
		Logger::LogWarning("value trovata");
	}
	catch(const std::exception& e) 	{
		Logger::LogWarning("eccezzasdasdsadsad");
		value = "0";
		Logger::LogWarning("dio nasone");
	}
	Logger::LogWarning("ritorno longgggg");
	res = StringUtils::StringToLongLong(value);
	Logger::LogWarning("fine ritorno longgggg");
	if(res < 0)
	{
		Logger::LogWarning(" ritorno longgggg negativo");
		return 0;
	}
	Logger::LogWarning(" ritorno longgggg positivo");
	return res;
}

bool HttpMessage::IsMessageComplete() const
{
	unsigned long long contentLength = GetContentLength(); 

	if (contentLength == 0)
		return header.size() > 0;

	return body.size() > contentLength;
}

long long HttpMessage::size() const
{
	int headerSize = 0;

	if (header.size() > 0)
	{
		for (Header::const_iterator it = header.begin(); it != header.end(); ++it)
			headerSize += it->first.size() + it->second.size() + 1;
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
