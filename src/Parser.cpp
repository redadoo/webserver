#include "Parser.hpp"

void Parser::TryParse(std::vector<ServerInfo> &serverInfos, const std::vector<Token> &tokens)
{
    (void)tokens;

    serverInfos.push_back(ServerInfo
    (
        8002,
        2,
        "index.html",
        "locahost",
        "web-page",
        "127.0.0.1",
        "web-page/error-pages/404.html"
    ));

    Logger::Log("finished parsing Configuration file");
}
