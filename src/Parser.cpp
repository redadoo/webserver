# include "Parser.hpp"
# include <Logger.hpp>

void Parser::FillServer(std::vector<Server> &Servers, const char *fileConf)
{
	std::vector<Token> tokens = Lexer::GetToken(fileConf);
	
	Servers.push_back(Server
	(
		8002,
		"9999",
		"index.html",
		"locahost",
		"web-page",
		"127.0.0.1",
		"web-page/error-pages/404.html"
	));

	Servers.push_back(Server
	(
		8003,
		"9999",
		"index.html",
		"locahost",
		"web-page",
		"127.0.0.1",
		"web-page/error-pages/404.html"
	));

	Logger::Log("finished parsing Configuration file");
}
