#include <WebServer.hpp>

void WebServer::StartServer() {}

void WebServer::InitServer() 
{
	for (size_t i = 0; i < serverInfo.size(); i++)
	{
		serverInfo[i].InitInfo();
	}
}

WebServer::WebServer()
{
	try
	{
		Logger::Log("start parsing Configuration file : config/default.conf");
		std::vector<Token> tokens = Lexer::GetToken(DEFAULT_CONFIG_FILE);
		Parser::TryParse(serverInfo, tokens);
		Logger::Log("finished parsing Configuration file");
		tokens.clear();
	}
	catch(const std::exception& e)
	{
		Logger::LogException(e);
	}
}

WebServer::WebServer(const char *fileConf)
{
	try
	{
		Logger::Log("start parsing Configuration file : config/default.conf");

		std::vector<Token> tokens = Lexer::GetToken(fileConf);
		Parser::TryParse(serverInfo, tokens);

		Logger::Log("finished parsing Configuration file");
	}
	catch(const std::exception& e)
	{
		Logger::LogException(e);
	}
}

WebServer::~WebServer() {}
