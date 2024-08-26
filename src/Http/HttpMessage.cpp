#include <HttpMessage.hpp>
#include <Logger.hpp>
#include <WebServerException.hpp>

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
		throw std::invalid_argument("Error: unexpected response");

	if(startLine.path.length() > 1024)
		throw std::invalid_argument("Error: unexpected response");

	if(startLine.path.find("../") != std::string::npos || startLine.path == ".." )
		throw std::invalid_argument("Error: unexpected response");

	if(startLine.httpVersion != "HTTP/1.1")
		throw std::invalid_argument("Error: unexpected response");

	if (startLine.httpMethod == "GET" || startLine.httpMethod == "DELETE")
		return;

	this->startLine.isInit = true;
}

// public function

void HttpMessage::ParseMessage(const std::string& messageChunk)
{
    std::string messageLine;
    std::istringstream messageChunkStream(messageChunk.c_str());

    while (std::getline(messageChunkStream, messageLine))
    {
        messageLine.erase(0, messageLine.find_first_not_of(" \t"));
        messageLine.erase(messageLine.find_last_not_of(" \t") + 1);

        if (!this->startLine.isInit)
        {
            ParseStartLine(messageLine);
            continue;
        }

        if (messageLine.empty())
            break;

        size_t pos = messageLine.find(": ");
        if (pos != std::string::npos)
        {
            std::string key = messageLine.substr(0, pos);
            std::string value = messageLine.substr(pos + 2);
            header.insert(std::make_pair(key, value));
        }
    }

    std::string bodyLine;
    while (std::getline(messageChunkStream, bodyLine))
        body += bodyLine + "\n";

    if (!body.empty() && body[body.size() - 1] == '\n')
        body.erase(body.size() - 1);
}

const char *HttpMessage::c_str() const
{
    std::string msg;

    msg.append(startLine.ToString());

    for (Header::const_iterator it = header.begin(); it != header.end(); ++it) 
    {
        msg.append(it->first);
        msg.append(it->second);
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
	os << "response message :\n" << msg.c_str();
	return (os);
}