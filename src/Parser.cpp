#include "Parser.hpp"

void Parser::TryParse(std::vector<ServerInfo> &serverInfos, const std::vector<Token> &tokens)
{
    (void)serverInfos;
    (void)tokens;
    throw ErrorOnFileConfigurationSyntax();
}
