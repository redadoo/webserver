#ifndef PARSER_HPP
#define PARSER_HPP

# include <ServerInfo.hpp>
# include <Lexer.hpp>

using namespace Lexer;

namespace Parser
{
    void FillServerInfo(std::vector<ServerInfo>& serverInfos, const char *fileConf);
}

#endif