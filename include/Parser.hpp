#ifndef PARSER_HPP
#define PARSER_HPP

#include <ServerInfo.hpp>
#include <Lexer.hpp>

using namespace Lexer;

namespace Parser
{
    void TryParse(std::vector<ServerInfo>& serverInfos, const std::vector<Token>& tokens);


    class ErrorOnFileConfigurationSyntax : public std::exception {
        public:
			virtual const char* what() const throw() { return "error when try to parse configuration file";}
	};
}

#endif