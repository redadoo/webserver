#include "utils.hpp"

bool utils::CheckFileExistence(const char *fileName)
{
    std::ifstream file(fileName);
    return file.good();
}

std::string utils::IntToString(int n)
{
    std::stringstream ss;      
    ss << n;                 
    return ss.str();
}
