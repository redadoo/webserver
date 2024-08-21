#ifndef LOCATION_HPP
#define LOCATION_HPP

# include <vector>
# include <string>
# include <CodePath.hpp>

struct Location
{
	std::vector<std::string> methods;
	std::vector<std::string> index;
	CodePath redirect;
	std::string path;
	std::string rootPath;
	std::string cgiExtension;
	std::string cgiPath;
	std::string uploadPath;
	bool autoIndex;
	bool uploadEnable;
};

#endif