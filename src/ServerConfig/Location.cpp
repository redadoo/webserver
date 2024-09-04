#include "Location.hpp"

bool Location::MatchesPath(const std::string &requestPath) const
{
	return requestPath.length() >= path.length() &&
			requestPath.compare(0, path.length(), path) == 0 &&
			(requestPath.length() == path.length() || requestPath[path.length()] == '/');
}

bool Location::IsMethodAllowed(const std::string &method) const
{
	if (methods.empty())
		return true;

	for (size_t i = 0; i < methods.size(); ++i)
	{
		if (methods[i] == method)
			return true;
	}
	return false;
}

bool Location::ShouldRedirect() const
{
	return redirect.isSet();
}

std::string Location::GetFilePath(const std::string& requestPath, const std::string& serverRoot) const
{
	std::string basePath = this->rootPath.empty() ? serverRoot : this->rootPath;

	while (!basePath.empty() && basePath[basePath.length() - 1] == '/')
		basePath.erase(basePath.length() - 1);

	if (this->path != "/" && basePath[basePath.length() - 1] != '/')
		basePath += this->path;

	std::string relativePath = requestPath.substr(this->path.length());
	while(!relativePath.empty() && relativePath[0] == '/')
		relativePath = relativePath.substr(1);

	std::string fullPath = basePath + "/" + relativePath;

	size_t pos;
	while (pos = fullPath.find("//"), pos != std::string::npos)
		fullPath.erase(pos, 1);

	if (fullPath[fullPath.length() - 1] != '/')
		fullPath += "/";

	return fullPath;
}

std::ostream &operator<<(std::ostream &os, const Location &loc)
{
    os << "Path: " << loc.path << "\n";
    os << "Root Path: " << loc.rootPath << "\n";
    os << "CGI Extension: " << loc.cgiExtension << "\n";
    os << "CGI Path: " << loc.cgiPath << "\n";
    os << "Upload Path: " << loc.uploadPath << "\n";
    os << "Auto Index: " << (loc.autoIndex ? "Enabled" : "Disabled") << "\n";
    os << "Upload Enable: " << (loc.uploadEnable ? "Enabled" : "Disabled") << "\n";
    os << "Upload Enable Find: " << (loc.uploadEnableFind ? "Enabled" : "Disabled") << "\n";
    os << "Methods: ";
    for (size_t i = 0; i < loc.methods.size(); ++i)
    {
        os << loc.methods[i];
        if (i < loc.methods.size() - 1)
            os << ", ";
    }
    os << "\nIndex: ";
    for (size_t i = 0; i < loc.index.size(); ++i)
    {
        os << loc.index[i];
        if (i < loc.index.size() - 1)
            os << ", ";
    }
    os << "\nRedirect: " << loc.redirect << "\n";
    return os;
}
