#ifndef PARSER_HPP
# define PARSER_HPP

# include <Server.hpp>
# include <Lexer.hpp>

using namespace Lexer;

namespace Parser
{
    void FillServer(std::vector<Server>& Servers, const char *fileConf);
}

#endif