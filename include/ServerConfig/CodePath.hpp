#pragma once

#ifndef CODEPATH_HPP
# define CODEPATH_HPP

#include <iostream>
#include <HttpStatusCode.hpp>

using namespace HttpStatusCode;

struct CodePath
{
	Code		code;
	std::string path;


	//default constructor
	CodePath() : code(toSet), path("") {}

	// Constructor that initializes with a Code type
	CodePath(Code _code, const std::string& _path);

	// Constructor that initializes with an integer code
	CodePath(int _code, const std::string &_path);

	/// @brief Checks if the `path` attribute of `CodePath` is set (i.e., not empty).
	/// @return `true` if the path is not empty; `false` otherwise.
	bool isSet() const;

	// Overload the << operator
	friend std::ostream& operator<<(std::ostream& os, const CodePath& cp);
};

#endif // CODEPATH_HPP
