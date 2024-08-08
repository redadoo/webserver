# include <WebServer.hpp>
# include <WebServerSignal.hpp>
# include <cstdlib>

WebServerSignal::SignalState signalState;

void WebServer::StartServer()
{
	while (!this->needToStop) 
	{
		if (signalState.signCaught)
			this->needToStop = true;
	}
}

void WebServer::InitServer()
{
	WebServerSignal::SetupSignalHandler();
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
