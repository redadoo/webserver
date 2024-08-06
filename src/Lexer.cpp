#include <Lexer.hpp>

std::vector<Lexer::Token> Lexer::GetToken(std::string fileName)
{
    std::string                 line;
    std::vector<Lexer::Token>   tokens;

    if (!utils::CheckFileExistence(fileName.c_str())) 
        throw FileNotFound();

    std::ifstream file(fileName.c_str());

    file.close();

    return tokens;
}
