#include "Body.hpp"
#include <string>
#include <Logger.hpp>

Body &Body::operator=(const Ustring &body) 
{
    this->content = body;
	return *this;
}

Body &Body::operator=(const std::string &body)
{
    this->content = body;
	return *this;
}

std::string Body::GetFileName() const
{
    return std::string();
}

std::string Body::GetBoundary() const
{
    return std::string();
}

size_t Body::size() const
{
    return this->content.size();
}

Ustring Body::GetFileContent() const
{
    return this->content;
}

std::ostream &operator<<(std::ostream &os, const Body &msg)
{
    for (size_t i = 0; i < msg.content.size(); ++i)
        os << static_cast<char>(msg.content[i]);
    return os;
}
