#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <vector>
#include <utils.hpp>

namespace Lexer
{
    enum TokenType
    {
        startServerContext,
        endServerContext,
        locationContext,
        simpleToken
    };

    struct Token
    {
        TokenType tokenType;
        std::string tokenName;
        std::string tokenValue;
    };

    std::vector<Token> GetToken(const char* fileName);


    class FileNotFound : public std::exception {
        public:
			virtual const char* what() const throw() { return "error: file not found";}
	};
}

#endif