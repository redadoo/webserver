#include <FIleUtils.hpp>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <algorithm>
#include <iostream>
#include <fcntl.h>
#include <Logger.hpp>

bool FileUtils::CheckFileExistence(const char *fileName)
{
	struct stat statbuf;
	return stat(fileName, &statbuf) == 0;
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

bool FileUtils::WriteFile(const std::string &fileName, const std::string &content)
{
	std::ofstream file(fileName.c_str());
	if (!file.is_open())
		return false;
	file << content;
	file.close();
	return true;
}

std::string FileUtils::GetContentType(const std::string &fileName)
{
	std::string extension = fileName.substr(fileName.find_last_of('.') + 1);
	std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
	if (extension == "html" || extension == "htm")
		return "text/html";

	return "text/plain";
}

int FileUtils::CheckFd(int fd)
{
	if (fcntl(fd, F_GETFD) < 0) return -1;
	return 0;
}