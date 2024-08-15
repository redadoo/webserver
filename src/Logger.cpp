# include "Logger.hpp"
# include <utils.hpp>

std::ofstream Logger::logFile;
std::string Logger::logFileName;

void Logger::Init(const std::string &fileName)
{
	logFileName = fileName;
	DeleteLog();
	logFile.open(logFileName.c_str(), std::ios::app);
	if (!logFile.is_open())
		throw std::runtime_error("Unable to open log file: " + logFileName);
}

void Logger::Shutdown()
{
	if (logFile.is_open())
		logFile.close();
}

void Logger::DeleteLog()
{
	Shutdown();
	std::remove(logFileName.c_str());
}

std::string Logger::CurrentDateTime()
{
	time_t	now;
	tm		*ltm;

	now = time(0);
	ltm = localtime(&now);
	std::stringstream ss;
	ss << (1900 + ltm->tm_year) << "-" << (1
		+ ltm->tm_mon) << "-" << ltm->tm_mday << " " << (ltm->tm_hour) << ":" << (ltm->tm_min) << ":" << (ltm->tm_sec);
	return (ss.str());
}

void Logger::WriteCurrentDataTime()
{
	if (logFile.is_open())
		logFile << "[" << CurrentDateTime() << "] ";
	else
		std::cerr << "Log file is not open." << std::endl;
}

void Logger::Log(const std::string &message)
{
	WriteCurrentDataTime();
	if (logFile.is_open())
		logFile << message << std::endl;
	else
		std::cerr << "Log file is not open." << std::endl;
}

void Logger::ClientLog(const Server& server, const Client &client, const std::string &msg)
{
	WriteCurrentDataTime();
	
	if (logFile.is_open())
	{
    	logFile << "Client " << client.srcIp << ":" << client.srcPort << " " << msg << 
		" by the socket: " << server.serverConfig.host << ":" << server.serverConfig.serverPort << std::endl;
	}
	else
		std::cerr << "Log file is not open." << std::endl;
}

void Logger::StartResponseLog(const Server &server, const Client &client)
{
	WriteCurrentDataTime();

	if (logFile.is_open())
	{
		logFile << "Client " << client.srcIp << ":" << client.srcPort << " send bytes to socket " << server.serverConfig.host << ":" << server.serverConfig.serverPort << std::endl;
	}
	else
		std::cerr << "Log file is not open." << std::endl;
}

void Logger::ResponseLog(const std::string &respone)
{
	if (logFile.is_open())
	{
		logFile << "[ StartResponseBlock ] " << std::endl << respone << "[ EndResponseBlock ]" << std::endl;
	}
	else
		std::cerr << "Log file is not open." << std::endl;
}

void Logger::EmptyLog(const std::string &msg)
{
	if (logFile.is_open())
    	logFile << msg << std::endl;
	else
		std::cerr << "Log file is not open." << std::endl;
}

void Logger::ServerLog(const Server &server, const std::string &msg)
{
	WriteCurrentDataTime();
	
	if (logFile.is_open())
	{
    	logFile << msg << " Socket " << server.serverConfig.host << ":" << server.serverConfig.serverPort << std::endl;
	}
	else
		std::cerr << "Log file is not open." << std::endl;
}

void Logger::LogWarning(const std::string &message)
{
	Log("WARNING: " + message);
}

void Logger::LogError(const std::string &message)
{
	Log("ERROR: " + message);
}

void Logger::LogException(const std::exception &ex)
{
	LogError(std::string("Exception: ") + ex.what());
}
