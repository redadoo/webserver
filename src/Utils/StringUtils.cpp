#include <StringUtils.hpp>
#include <sstream>     
#include <cstring>

std::string StringUtils::ToString(int n)
{
	std::stringstream ss;      
	ss << n;                 
	return ss.str();
}

std::string StringUtils::ToString(uint16_t n)
{
	std::stringstream ss;
	ss << n;                 
	return ss.str();
}

int StringUtils::StrintToInt(const std::string& str)
{
	int number;

	for (size_t i = 0; i < str.size(); i++)
	{
		if (!isdigit(str[i]))
			throw std::invalid_argument("Invalid string to convert to int");
	}

	std::stringstream ss(str);
	ss >> number;
	return number;
}

std::vector<std::string> StringUtils::Split(const std::string& str, char delim)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(str);
	
	while (std::getline(tokenStream, token, delim))
		tokens.push_back(token);
	return tokens;
}

std::vector<std::string> StringUtils::Split(const std::string &str, const std::string &delim)
{
	std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = str.find(delim);

    while (end != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + delim.length();
        end = str.find(delim, start);
    }

    // Add the last token
    tokens.push_back(str.substr(start));

    return tokens;
}

bool StringUtils::IsAllDigit(const std::string &str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if(!isdigit(str[i])) 
			return false;
	}
	
    return true;
}
