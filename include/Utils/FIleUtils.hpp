#pragma once

#ifndef FILEUTILS_HPP
# define FILEUTILS_HPP

#include <Ustring.hpp>

namespace FileUtils
{
	/// @brief Checks if a file exists at the given path.
	/// @param fileName The path to the file to check.
	/// @return `true` if the file exists, `false` otherwise.
	bool CheckFileExistence(const char* fileName);

	/// @brief Checks if the specified path corresponds to a directory.
	/// @param path The path to be checked.
	/// @return `true` if the path is a directory; `false` otherwise.
	bool IsDirectory(const char* path);

	/// @brief Reads the content of a file into a string.
	/// @param fileName The path to the file to be read.
	/// @return A string containing the file content. Returns an empty string if the file cannot be opened.
	std::string ReadFile(const std::string& fileName);

	/// @brief Reads the content of a file into a Ustring.
	/// @param fileName The path to the file to be read.
	/// @exception Throws a runtime_error if the file cannot be opened or has invalid size.
	/// @return A Ustring containing the file content.
	Ustring ReadBinaryFile(const std::string &fileName);

	/// @brief Writes the specified content to a file in text or binary mode.
	/// @param fileName The path to the file where the content will be written.
	/// @param content The content to be written to the file.
	/// @param isBinary Set to true to write in binary mode, false for text mode.
	/// @return `true` if the file was successfully opened and the content was written; `false` otherwise.
	bool WriteFile(const std::string& fileName, const Ustring& content, const bool isBinary);

	/// @brief Determines the MIME content type based on the file extension.
	/// @param fileName The name of the file whose content type is to be determined.
	/// @return The MIME content type as a string. Returns `"text/html"` for `.html` and `.htm` extensions, and `"text/plain"` for other extensions.
	std::string GetContentType(const std::string& fileName);

    /// @brief Checks if a file descriptor is valid.
    /// @param fd The file descriptor to be checked.
    /// @return `0` if the file descriptor is valid, `-1` otherwise.
	int CheckFd(const int fd);

} // namespace FIleUtils

#endif // !FILEUTILS_HPP
