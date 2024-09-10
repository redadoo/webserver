#ifndef LOGGER_HPP
# define LOGGER_HPP

#include <string>
#include <exception>
#include <fstream>  
#include <ctime>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdint.h>
#include <Server.hpp>

/// @brief The Logger class provides static logging functionality for applications,
/// allowing messages, warnings, errors, and exceptions to be recorded in a log file.
class Logger 
{
private:
	static bool				isEnable;
	static std::ofstream 	logFile;
	static std::string 		logFileName;

	/// @brief Gets the current date and time as a formatted string.
	/// @return A string representing the current date and time.
	static std::string CurrentDateTime();

	/// @brief Write the current date and time to the log file without newline
	static void WriteCurrentDataTime();

public:
	/// @brief Initializes the log file. This must be called before any logging can be done.
	static void Init(const std::string& fileName = "webserver.log");

	/// @brief Logs a general message to the log file.
	/// @param message The message to log.
	static void Log(const std::string &message);

	/// @brief Logs a client-specific message to the log file.
	/// @param server The server handling the client.
	/// @param client The client instance associated with the log.
	/// @param msg The specific message to log for the client.
	static void ClientLog(const Server& Server, const Client& client,const char *msg);

	/// @brief Logs the details of an HTTP response to the log file.
	/// @param server The server sending the response.
	/// @param client The client receiving the response.
	/// @param response The HTTP response to log.
	static void ResponseLog(const Server &server, const Client &client, const char* respone);

	/// @brief Logs the details of an HTTP request to the log file.
	/// @param server The server receiving the request.
	/// @param client The client sending the request.
	/// @param request The HTTP request to log.
	static void RequestLog(const Server &server, const Client &client, const HttpMessage& request);

	/// @brief Logs a raw request string to the log file.
	/// @param server The server receiving the request.
	/// @param client The client sending the request.
	/// @param request The raw HTTP request string to log.
	static void RequestLog(const Server &server, const Client &client, const std::string& request);

	/// @brief Logs a server-specific message to the log file.
	/// @param server The server instance associated with the log.
	/// @param msg The specific message to log for the server.
	static void ServerLog(const Server& Server, const std::string &msg);

	/// @brief Logs a warning message to the log file with red color formatting.
	/// @param message The warning message to log.
	static void LogWarning(const std::string &message);

	/// @brief Logs an error message to the log file.
	/// @param message The error message to log.
	static void LogError(const std::string &message);

	/// @brief Logs an exception's details to the log file as an error.
	/// @param ex The exception to log.
	static void LogException(const std::exception &ex);
	
	/// @brief Closes the log file if it is open.
	static void Shutdown();

	/// @brief Deletes the current log file.
	static void DeleteLog();
};

#endif // LOGGER_HPP
