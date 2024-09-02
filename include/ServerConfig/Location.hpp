#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <vector>
#include <string>
#include <iostream>
#include <CodePath.hpp>

struct Location
{
	std::vector<std::string> 	methods;
	std::vector<std::string>	index;
	CodePath					redirect;
	std::string 				path;
	std::string 				rootPath;
	std::string 				cgiExtension;
	std::string 				cgiPath;
	std::string 				uploadPath;
	bool 						autoIndex;
	bool 						isInitAutoindex;
	bool 						uploadEnable;
	bool 						uploadEnableFind;

	Location(const std::string& _path) : path(_path), rootPath(""), isInitAutoindex(false) {}

	bool MatchesPath(const std::string& requestPath) const;
	bool IsMethodAllowed(const std::string& method) const;
	bool ShouldRedirect() const;

	std::string GetFilePath(const std::string& requestPath, const std::string& serverRoot) const;

	// Overload the << operator
	friend std::ostream& operator<<(std::ostream& os, const Location& loc);
};

#endif // LOCATION_HPP
