#include "Logger.hpp"

// global variable

std::ofstream Logger::logFile;
std::string Logger::logFileName;

//private function

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

//public function

void Logger::Init(const std::string &fileName)
{
	logFileName = fileName;
	DeleteLog();
	logFile.open(logFileName.c_str(), std::ios::app);
	if (!logFile.is_open())
		throw std::runtime_error("Unable to open log file: " + logFileName);
}

void Logger::Log(const std::string &message)
{
	WriteCurrentDataTime();
	if (logFile.is_open())
		logFile << message << std::endl;
	else
		std::cerr << "Log file is not open." << std::endl;
}

void Logger::ClientLog(const Server& server, const Client &client, const char *msg)
{
	WriteCurrentDataTime();
	
	if (logFile.is_open())
	{
    	logFile << "Client " << client.clientConfig.ip << ":" << client.clientConfig.port << " " << msg << 
		" by the socket: " << server.serverConfig.host << ":" << server.serverConfig.serverPort.port << std::endl;
	}
	else
		std::cerr << "Log file is not open." << std::endl;
}

void Logger::RequestLog(const Server &server, const Client &client, const HttpMessage &request)
{
	WriteCurrentDataTime();

	if (logFile.is_open())
	{
		logFile << "Client " << client.clientConfig.ip << client.clientConfig.port << 
		" send a request to socket " << server.serverConfig.host << server.serverConfig.serverPort.port << std::endl;

		logFile << "[ StartRequestBlock ] " << std::endl << request << "[ EndRequestBlock ]" << std::endl;
	}
	else
		std::cerr << "Log file is not open." << std::endl;
}

void Logger::RequestLog(const Server &server, const Client &client, const std::string &request)
{
	WriteCurrentDataTime();

	if (logFile.is_open())
	{
		logFile << "Client " << client.clientConfig.ip << ":" << client.clientConfig.port << 
			" send a request to socket " << server.serverConfig.host << ":" << server.serverConfig.serverPort.port << std::endl;
		logFile << "[ StartRequestBlock ] " << std::endl << request << "[ EndRequestBlock ]" << std::endl;
	}
	else
		std::cerr << "Log file is not open." << std::endl;
}

void Logger::ResponseLog(const Server &server, const Client &client, const char* httpResponse)
{
	if (logFile.is_open())
	{
		logFile << "Server " << server.serverConfig.socketIp << ":" << server.serverConfig.serverPort.port << 
			" send a response to client " << client.clientConfig.ip << ":" << client.clientConfig.port << std::endl;
		logFile << "[ StartResponseBlock ] " << std::endl << httpResponse << "[ EndResponseBlock ]" << std::endl;
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
    	logFile << msg << " Socket " << server.serverConfig.host << ":" << server.serverConfig.serverPort.port << std::endl;
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