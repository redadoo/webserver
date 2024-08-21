#ifndef CODEPATH_HPP
#define CODEPATH_HPP

# include <string>
# include <iostream> // Include this for std::ostream

struct CodePath
{
	int code;
	std::string path;

	// Default constructor
	CodePath() : code(0), path("") {}

	// Parameterized constructor
	CodePath(int _code, std::string _path) : code(_code), path(_path) {}

	// Overload the << operator
	friend std::ostream& operator<<(std::ostream& os, const CodePath& cp)
	{
		os << "Code: " << cp.code << ", Path: " << cp.path;
		return os;
	}
};

#endif // CODEPATH_HPP
