#include <WebServer.hpp>

void WebServer::StartServer() {}

WebServer::WebServer()
{
    try
    {
        std::vector<Token> tokens = Lexer::GetToken(DEFAULT_CONFIG_FILE);
        Parser::TryParse(serverInfo, tokens);
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
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

WebServer::~WebServer() {}
