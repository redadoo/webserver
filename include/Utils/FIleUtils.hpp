#pragma once
#ifndef FILEUTILS_HPP
# define FILEUTILS_HPP

#include <string>
#include <vector>
#include <stdint.h>
#include <Ustring.hpp>

namespace FileUtils
{
    enum FileType { text, binary };

    /// @brief Checks if a file exists at the given path.
    /// @param fileName The path to the file to check.
    /// @return True if the file exists, false otherwise.
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
    /// @return A Ustring containing the file content. Returns an empty Ustring if the file cannot be opened.
    Ustring ReadBinaryFile(const std::string &fileName);

    /// @brief Writes the specified content to a file.
    /// @param fileName The path to the file where the content will be written.
    /// @param content The content to be written to the file.
    /// @return `true` if the file was successfully opened and the content was written; `false` otherwise.
	bool WriteFile(const std::string& fileName, const std::string& content);

    /// @brief Writes the specified content to a file.
    /// @param fileName The path to the file where the content will be written.
    /// @param content The content to be written to the file.
    /// @return `true` if the file was successfully opened and the content was written; `false` otherwise.
	bool WriteFile(const std::string& fileName, const Ustring& content);

    std::string GetFileExtension(const std::string& fileName);

    /// @brief Determines the MIME content type based on the file extension.
    /// @param fileName The name of the file whose content type is to be determined.
    /// @return The MIME content type as a string. Returns `"text/html"` for `.html` and `.htm` extensions, and `"text/plain"` for other extensions.
	std::string GetContentType(const std::string& fileName);

    int CheckFd(int fd);

    FileType GetFileType(const std::string& fileName);


} // namespace FIleUtils

#endif
