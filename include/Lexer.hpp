#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <vector>

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
}

#endif