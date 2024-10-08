#ifndef BODY_HPP
# define BODY_HPP

#include <vector>
#include <stdint.h>
#include <string>
#include <Ustring.hpp>

class Body
{
	public:
		Ustring    content;

	    Body& operator=(const Ustring& body);
	    Body& operator=(const std::string& body);
	    friend std::ostream& operator<<(std::ostream& os, const Body& msg);

		std::string				GetFileName() const;
		Ustring	GetFileContent() const;
		std::string				GetBoundary() const;
		size_t					size() const ;
};

#endif