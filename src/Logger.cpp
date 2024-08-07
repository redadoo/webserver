#include "Logger.hpp"
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

Logger::Logger() : logFileName("log.txt")
{
	std::ofstream ofs(logFileName.c_str(), std::ios::out | std::ios::trunc);

	logFile.open(logFileName.c_str(), std::ios::app);
	if (!logFile.is_open())
	{
		throw std::runtime_error("Unable to open log file: " + logFileName);
	}
}

Logger::~Logger()
{
	if (logFile.is_open())
	{
		logFile.close();
	}
}

void Logger::DeleteLog()
{
    if (logFile.is_open()) logFile.close();

    if (std::remove(logFileName.c_str()) != 0) {
        std::cerr << "Error deleting file: " << logFileName << std::endl;
    } else {
        std::cout << "Log file deleted successfully: " << logFileName << std::endl;
    }
}

std::string Logger::CurrentDateTime()
{
	time_t	now;
	tm		*ltm;

	now = time(0);
	ltm = localtime(&now);
	std::stringstream ss;
	ss << (1900 + ltm->tm_year) << "-" << (1
		+ ltm->tm_mon) << "-" << ltm->tm_mday << " " << (1
		+ ltm->tm_hour) << ":" << (1 + ltm->tm_min) << ":" << (1 + ltm->tm_sec);
	return (ss.str());
}

std::string colorRed(const std::string &message) {
    std::string red = "\033[31m"; 
    std::string reset = "\033[0m"; 
    return red + message + reset;
}


void Logger::Log(const std::string &message)
{
	if (logFile.is_open())
		logFile << "[" << CurrentDateTime() << "] " << message << std::endl;
	else
		std::cerr << "Log file is not open." << std::endl;
}

void Logger::LogWarning(const std::string &message) {
    Log(colorRed("WARNING: " + message));
}

void Logger::LogError(const std::string &message) {
    Log(colorRed("ERROR: " + message));
}

void Logger::LogException(const std::exception &ex) {
    LogError(std::string("Exception: ") + ex.what());
}
