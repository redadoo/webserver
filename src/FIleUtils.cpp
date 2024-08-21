#include <FIleUtils.hpp>

bool FIleUtils::CheckFileExistence(const char *fileName)
{
	std::ifstream file(fileName);
	return file.good();
}