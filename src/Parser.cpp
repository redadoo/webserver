#include "Parser.hpp"

void Parser::FillServerInfo(std::vector<ServerInfo> &serverInfos, const char *fileConf)
{
    // std::vector<Token> tokens = Lexer::GetToken(fileConf);
    (void)fileConf;
    
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

    Logger::Log("finished parsing Configuration file");
}
