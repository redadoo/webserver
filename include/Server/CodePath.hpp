#ifndef CODEPATH_HPP
#define CODEPATH_HPP

# include <string>
# include <iostream> // Include this for std::ostream
# include <HttpStatusCode.hpp>

using namespace HttpStatusCode;

struct CodePath
{
	Code		code;
	std::string path;


	CodePath() : code(toSet), path("") {}
	CodePath(Code _code, const std::string& _path);
	CodePath(int _code, const std::string &_path);


	// Overload the << operator
	friend std::ostream& operator<<(std::ostream& os, const CodePath& cp);
};

#endif // CODEPATH_HPP
