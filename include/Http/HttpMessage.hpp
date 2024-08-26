#ifndef HTTPMESSAGE_HPP
# define HTTPMESSAGE_HPP

#include <map>
#include <string>
#include <iostream>
#include <sstream>

struct StartLine
{
	std::string					httpMethod;
	std::string					httpVersion;
	std::string					path;
	bool 						isInit;

	size_t size() const {return httpMethod.size() + httpVersion.size() + path.size();}
};

struct CaseInsensitiveCompare {
	bool operator()(const std::string& a, const std::string& b) const;
	static bool char_compare(char ac, char bc);
};

typedef std::map<std::string, std::string, CaseInsensitiveCompare> Header;

class HttpMessage
{
public:
	StartLine 	startLine;
	Header      header;
	std::string body;
	
	void ParseMessage(const std::string messageChunk);

	friend std::ostream& operator<<(std::ostream& os, const HttpMessage& msg);
	size_t size() const;
	const char *c_str() const;

private:

	void ParseStartLine(const std::string& str);

};

#endif