# include "Parser.hpp"
# include <Logger.hpp>

void Parser::FillServerInfo(std::vector<ServerInfo> &serverInfos, const char *fileConf)
{
    std::vector<Token> tokens = Lexer::GetToken(fileConf);
    
    serverInfos.push_back(ServerInfo
    (
        8002,
        "9999",
        "index.html",
        "locahost",
        "web-page",
        "127.0.0.1",
        "web-page/error-pages/404.html"
    ));

    serverInfos.push_back(ServerInfo
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
