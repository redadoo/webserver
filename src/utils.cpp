#include "utils.hpp"

bool utils::CheckFileExistence(const char *fileName)
{
    std::ifstream file(fileName);
    return file.good();
}