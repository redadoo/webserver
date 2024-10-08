#include "Header.hpp"

Header::iterator Header::find(const std::string &key)
{
	return content.find(key);
}

std::string &Header::operator[](const std::string &key)
{
	return content[key];
}

std::string Header::at(const std::string& key) const
{
	return content.at(key);
}

size_t Header::size() const
{
	size_t sizeHeader = 0;
	std::map<std::string, std::string>::const_iterator it;

	for (it = begin(); it != end(); it++)
	{
		sizeHeader += it->first.size();
		sizeHeader += it->second.size();
	}

	return sizeHeader;
}
