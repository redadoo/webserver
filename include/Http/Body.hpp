#pragma once

#ifndef BODY_HPP
# define BODY_HPP

#include <Ustring.hpp>

enum BodyType
{
	None,
	Plain,
	Binary		
};

class Body
{
	public:
		BodyType	bodyType;
		Ustring    	content;

	    Body& operator=(const Ustring& body);
	    Body& operator=(const std::string& body);
	    friend std::ostream& operator<<(std::ostream& os, const Body& msg);

		void		Init();
		size_t		size() const;
		bool		IsBinary() const;
		std::string	GetBoundary() const;
		std::string	GetFileName() const;
		Ustring		GetFileContent() const;
		void		FindType();
};

#endif