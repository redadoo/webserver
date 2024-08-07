#ifndef PARSER_HPP
#define PARSER_HPP

# include <ServerInfo.hpp>
# include <Lexer.hpp>

using namespace Lexer;

namespace Parser
{
    void TryParse(std::vector<ServerInfo>& serverInfos, std::vector<Token>& tokens);
}

#endif