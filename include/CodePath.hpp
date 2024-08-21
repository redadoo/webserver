#ifndef CODEPATH_HPP
# define CODEPATH_HPP

struct CodePath
{
	int code;
	std::string path;

	CodePath() : code(0), path("") {};
	CodePath(int _code, std::string _path) : code(_code), path(_path) {};
};

#endif