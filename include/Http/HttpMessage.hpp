#ifndef HTTPMESSAGE_HPP
# define HTTPMESSAGE_HPP

# include <map>
# include <string>
# include <iostream>

struct CaseInsensitiveCompare {
	bool operator()(const std::string& a, const std::string& b) const;
	static bool char_compare(char ac, char bc);
};

typedef std::map<std::string, std::string, CaseInsensitiveCompare> Header;

class HttpMessage
{
public:
	std::string url;
	Header      header;
	std::string body;
	
	void BuildMessage();
	const std::string &GetMessage(); 
	void ParseHeader();
	
	const char *c_str() const;
	size_t size() const;

	friend std::ostream& operator<<(std::ostream& os, const HttpMessage& msg);
};

#endif