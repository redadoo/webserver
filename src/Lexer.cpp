#include <Lexer.hpp>


std::vector<Lexer::Token> Lexer::GetToken(const char* fileName)
{
    if (!utils::CheckFileExistence(fileName)) 
        throw FileNotFound();
    return std::vector<Lexer::Token>();
}
