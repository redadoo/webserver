#include <Lexer.hpp>


std::vector<Lexer::Token> Lexer::GetToken(std::string fileName)
{
    std::string                 line;
    std::vector<Lexer::Token>   tokens;

    if (!utils::CheckFileExistence(fileName.c_str())) 
        throw FileNotFound();

    std::ifstream file(fileName.c_str());

    while (std::getline(file, line)) 
    {
        std::cout << line << std::endl;
    }

    file.close();

    return tokens;
}
