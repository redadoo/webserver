#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <exception>
#include <fstream>  

class Logger 
{
private:
    std::ofstream logFile;
    std::string logFileName;

    std::string CurrentDateTime();

public:
    Logger();
    ~Logger();

    void DeleteLog();
    void Log(const std::string &message);
    void LogWarning(const std::string &message);
    void LogError(const std::string &message);
    void LogException(const std::exception &ex);
};

#endif // LOGGER_HPP
