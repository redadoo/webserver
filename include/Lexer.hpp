#ifndef LEXER_HPP
# define LEXER_HPP

# include <string>
# include <vector>
# include <fstream>

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

    std::vector<Token> GetToken(std::string fileName);
}

#endif