#include <HttpMessage.hpp>

bool CaseInsensitiveCompare::char_compare(char ac, char bc)
{
	return (std::tolower(static_cast<unsigned char>(ac)) < std::tolower(static_cast<unsigned char>(bc)));
}

bool CaseInsensitiveCompare::operator()(const std::string &a,
	const std::string &b) const
{
	return (std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end(),
			CaseInsensitiveCompare::char_compare));
}

const std::string &HttpMessage::GetMessage()
{
	std::string msg;
	msg.append(this->url);
	msg.append(this->header);
	msg.append(this->body);
}

void HttpMessage::ParseHeader()
{
	
}

const char *HttpMessage::c_str() const
{
	return (GetMessage().c_str());
}

size_t HttpMessage::size() const
{
	return (1);
}

std::ostream &operator<<(std::ostream &os, const HttpMessage &msg)
{
	os << "response message :\n" << msg.c_str();
	return (os);
}