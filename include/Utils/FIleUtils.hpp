#ifndef FILEUTILS_HPP
# define FILEUTILS_HPP

#include <string>

namespace FileUtils
{
    /// @brief Checks if a file exists at the given path.
    /// @param fileName The path to the file to check.
    /// @return True if the file exists, false otherwise.
    bool CheckFileExistence(const char* fileName);

	bool IsDirectory(const char* path);

	std::string ReadFile(const std::string& fileName);

	bool WriteFile(const std::string& fileName, const std::string& content);

	std::string GetContentType(const std::string& fileName);



} // namespace FIleUtils

#endif
