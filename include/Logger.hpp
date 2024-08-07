#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <exception>
#include <fstream>  
#include <ctime>
#include <iostream>
#include <sstream>

/// @brief The Logger class provides logging functionality for applications,
/// allowing messages, warnings, errors, and exceptions to be recorded in a log file.
class Logger 
{
private:
    std::ofstream logFile; ///< Output file stream for writing log entries.
    std::string logFileName; ///< Name of the log file.

    /// @brief Gets the current date and time as a formatted string.
    /// @return A string representing the current date and time.
    std::string CurrentDateTime();

public:
    Logger();
    ~Logger();

    /// @brief Deletes the current log file.
    void DeleteLog();

    /// @brief Logs a general message to the log file.
    /// @param message The message to log.
    void Log(const std::string &message);

    /// @brief Logs a warning message to the log file with red color formatting.
    /// @param message The warning message to log.
    void LogWarning(const std::string &message);

    /// @brief Logs an error message to the log file with red color formatting.
    /// @param message The error message to log.
    void LogError(const std::string &message);

    /// @brief Logs an exception's details to the log file as an error.
    /// @param ex The exception to log.
    void LogException(const std::exception &ex);
};

#endif // LOGGER_HPP
