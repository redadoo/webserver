#include <WebServer.hpp>

void WebServer::StartServer() {}

void WebServer::InitServer() {}


WebServer::WebServer()
{
	try
	{
		std::vector<Token> tokens = Lexer::GetToken(DEFAULT_CONFIG_FILE);
		Parser::TryParse(serverInfo, tokens);
		
		tokens.clear();

		InitServer();

	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}

WebServer::WebServer(const char *fileConf)
{
	try
	{
		std::vector<Token> tokens = Lexer::GetToken(fileConf);
		Parser::TryParse(serverInfo, tokens);
				
		tokens.clear();

		InitServer();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}

WebServer::~WebServer() {}
