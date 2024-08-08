# include <WebServer.hpp>
# include <WebServerSignal.hpp>
# include <cstdlib>

WebServerSignal::SignalState signalState;

void WebServer::StartServer()
{
	while (!this->needToStop) 
	{
		this->needToStop = signalState.signCaught;
	}
}

void WebServer::InitServer()
{
	try
	{
		WebServerSignal::SetupSignalHandler();

		for (size_t i = 0; i < serverInfo.size(); i++)
			serverInfo[i].InitInfo();		
	}
	catch(const std::exception& e)
	{
		Logger::LogException(e);
	}
	
}

void WebServer::ParseData(const char *fileConf)
{
	std::vector<Token> tokens = Lexer::GetToken(fileConf);
	Parser::TryParse(serverInfo, tokens);
}

WebServer::WebServer()
{
	this->needToStop = false;

	try
	{
		ParseData(DEFAULT_CONFIG_FILE);
	}
	catch (const std::exception &e)
	{
		Logger::LogException(e);
	}
}

WebServer::WebServer(const char *fileConf)
{
	this->needToStop = false;

	try
	{
		ParseData(fileConf);
	}
	catch (const std::exception &e)
	{
		Logger::LogException(e);
	}
}

WebServer::~WebServer() {}
