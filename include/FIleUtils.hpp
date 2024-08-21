#ifndef FILEUTILS_HPP
# define FILEUTILS_HPP

namespace FIleUtils
{
    /// @brief Checks if a file exists at the given path.
    /// @param fileName The path to the file to check.
    /// @return True if the file exists, false otherwise.
    bool CheckFileExistence(const char* fileName);
    
} // namespace FIleUtils

#endif