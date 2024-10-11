#pragma once
#ifndef LOCATION_HPP
# define LOCATION_HPP

#include <vector>
#include <string>
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

	/// @brief Determines if the given request path matches the location's path based on length and prefix criteria.
	/// @param requestPath The path of the incoming request.
	/// @return `true` if the request path starts with the location's path and either matches exactly or is followed by a `/`; `false` otherwise.
	bool MatchesPath(const std::string& requestPath) const;

	/// @brief Checks if the specified HTTP method is allowed for this location.
	/// @param method The HTTP method to check (e.g., "GET", "POST").
	/// @return `true` if the method is allowed; `false` if the method is not allowed or if no methods are specified (defaulting to allowed).
	bool IsMethodAllowed(const std::string& method) const;

	/// @brief Determines if a redirection is configured for this location.
	/// @return `true` if a redirection is set; `false` otherwise.
	bool ShouldRedirect() const;

	/// @brief Constructs the full filesystem path for the requested resource based on the request path and the location's root path.
	/// @param requestPath The path of the incoming request.
	/// @param serverRoot The server root directory used if the location's root path is not specified.
	/// @return The resulting full path, adjusted for any redundant slashes and appended with a trailing slash if needed.
	std::string GetFilePath(const std::string& requestPath, const std::string& serverRoot) const;

	// Overload the << operator
	friend std::ostream& operator<<(std::ostream& os, const Location& loc);
};

#endif // LOCATION_HPP
