#ifndef UTILS_HPP
#define UTILS_HPP

# include <iostream>
# include <fstream>
# include <sstream>
namespace utils 
{
    bool CheckFileExistence(const char* fileName);
    std::string IntToString(int n);
}

#endif