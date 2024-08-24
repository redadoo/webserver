# include <FIleUtils.hpp>
# include <fstream>

bool FileUtils::CheckFileExistence(const char *fileName)
{
	std::ifstream file(fileName);
	return file.good();
}