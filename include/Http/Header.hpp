#pragma once
#ifndef HEADER_HPP
# define HEADER_HPP

#include <map>
#include <string>

struct CaseInsensitiveCompare {
	bool operator()(const std::string& a, const std::string& b) const;
	static bool char_compare(char ac, char bc);
};

struct Header
{
	std::map<std::string, std::string, CaseInsensitiveCompare> content;
	
	// Typedef for iterators
	typedef std::map<std::string, std::string, CaseInsensitiveCompare>::const_iterator const_iterator;
	typedef std::map<std::string, std::string, CaseInsensitiveCompare>::iterator iterator;

	const_iterator begin() const {
		return content.begin();
	}

	const_iterator end() const {
		return content.end();
	}

	std::string& operator[](const std::string& key);
	iterator find(const std::string& key);

	std::string at(const std::string &key) const;
	size_t size() const;

    void erase(const std::string& key) {
        iterator it = content.find(key);
        if (it != content.end()) {
            content.erase(it);
        }
    }
};

#endif