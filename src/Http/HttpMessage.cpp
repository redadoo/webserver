#include <HttpMessage.hpp>
#include <Logger.hpp>
#include <WebServerException.hpp>
#include <StringUtils.hpp>

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

void HttpMessage::ParseMessage(const std::string& messageChunk)
{
	
	std::istringstream 			messageChunkStream(messageChunk);
	std::vector<std::string> 	messageLines = StringUtils::Split(messageChunk,"\r\n");
	bool 						isBody = false;

	ParseStartLine(messageLines[0]);

	for (size_t i = 1; i < messageLines.size(); i++)
	{
		if (messageLines[i].empty())
			isBody = true;

		if (!isBody)
		{
			size_t pos = messageLines[i].find(": ");
			if (pos != std::string::npos)
			{
				std::string key = messageLines[i].substr(0, pos + 1);
				std::string value = messageLines[i].substr(pos + 2);
				header.insert(std::make_pair(key, value));
			}
		}
		else
		{
			body += messageLines[i] + "\n";

			if (!body.empty() && body[body.size() - 1] == '\n')
				body.erase(body.size() - 1);
		}
	}
}

const char *HttpMessage::c_str() const
{
	std::string msg = "";

	msg.append(startLine.ToString());
	
	for (Header::const_iterator it = header.begin(); it != header.end(); ++it) 
	{
		msg.append(it->first);
		msg.append(" ");
		msg.append(it->second);
		msg.append("\n");
	}

	msg.append(body);
	
	return msg.c_str();
}

size_t HttpMessage::size() const
{
	return (startLine.size() + header.size() + body.size());
}

std::ostream &operator<<(std::ostream &os, const HttpMessage &msg)
{
	os << msg.c_str();
	return (os);
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
