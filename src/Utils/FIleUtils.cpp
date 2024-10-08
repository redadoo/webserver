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

Ustring FileUtils::ReadBinaryFile(const std::string &fileName)
{
    // Open file in binary mode
    std::ifstream instream(fileName.c_str(), std::ios::in | std::ios::binary);
    
    // Check if the file opened successfully
    if (!instream.is_open())
    {
        throw std::runtime_error("Failed to open file: " + fileName);
    }

    // Seek to the end of the file to determine the file size
    instream.seekg(0, std::ios::end);
    std::streampos fileSize = instream.tellg();
    instream.seekg(0, std::ios::beg);

    // Check for valid file size
    if (fileSize <= 0)
    {
        throw std::runtime_error("Invalid file size: " + fileName);
    }

    // Resize the vector to fit the file content
    std::vector<uint8_t> data(static_cast<size_t>(fileSize));

    // Read file content into the vector
    instream.read(reinterpret_cast<char*>(&data[0]), fileSize);
    
    // Close the file
    instream.close();

    // Return a Ustring object constructed from the binary data
    return Ustring(std::string(data.begin(), data.end()));
}

bool FileUtils::WriteFile(const std::string &fileName, const Ustring &content)
{
	std::ofstream file(fileName.c_str(), std::ios::out | std::ios::binary /* | std::ios::app */);
	if (!file.is_open())
		return false;
    file.write(reinterpret_cast<const char*>(content.get_content().data()), content.size());	
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