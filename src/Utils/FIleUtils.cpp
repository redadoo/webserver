#include <FIleUtils.hpp>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <algorithm>

bool FileUtils::CheckFileExistence(const char *fileName)
{
	std::ifstream file(fileName);
	return file.good();
}

bool FileUtils::IsDirectory(const char *path)
{
	struct stat statbuf;
	if (stat(path, &statbuf) != 0)
		return false;
	return S_ISDIR(statbuf.st_mode);
}

std::string FileUtils::ReadFile(const std::string &fileName)
{
	std::ifstream file(fileName.c_str());
	if (!file.is_open())
		return "";

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

std::string FileUtils::GetContentType(const std::string &fileName)
{
	std::string extension = fileName.substr(fileName.find_last_of('.') + 1);
	std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
	if (extension == "html" || extension == "htm")
		return "text/html";

	return "text/plain";
}

