#include <StringUtils.hpp>
#include <Logger.hpp>

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

std::string StringUtils::ToString(unsigned long long n)
{
	std::stringstream ss;
	ss << n;
	return ss.str();
}

std::string StringUtils::ToString(long long n)
{
	std::stringstream ss;
	ss << n;
	return ss.str();
}

std::string StringUtils::ToString(off_t n)
{
	std::stringstream ss;
	ss << n;
	return ss.str();
}

std::string StringUtils::ToString(size_t n)
{
	std::stringstream ss;
	ss << n;
	return ss.str();
}

int StringUtils::StrintToInt(const std::string& str)
{
	std::stringstream ss(str);
	unsigned long long number;
	ss >> number;
	if (ss.fail())
		throw std::runtime_error("Invalid string to convert to int");
	return number;
}

long long StringUtils::StringToLongLong(const std::string& str)
{
	long long number;

	for (size_t i = 0; i < str.size(); i++)
	{
		if (!isdigit(str[i]))
			throw std::invalid_argument("Invalid string to convert to unsigned long long");
	}

	std::stringstream ss(str);
	ss >> number;
	return number;
}

unsigned long long StringUtils::StringToUnsignedLongLong(const std::string& str)
{
	unsigned long long number;

	for (size_t i = 0; i < str.size(); i++)
	{
		if (!isdigit(str[i]))
			throw std::invalid_argument("Invalid string to convert to unsigned long long");
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

std::vector<std::string> StringUtils::Split(const std::string& s, const std::string &delimiter)
{
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
		token = s.substr (pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back (token);
	}

	res.push_back (s.substr (pos_start));
	return res;
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

std::string StringUtils::FormatTime(time_t time)
{
	struct tm *timeinfo;
	char buffer[80];

	timeinfo = localtime(&time);

	if (timeinfo == NULL)
		return "Unknown time";

	strftime(buffer, sizeof(buffer), "%d-%b-%Y %H:%M", timeinfo);
	return std::string(buffer);
}

std::pair<std::string, std::string> StringUtils::SplitPathAndQuery(const std::string &path)
{
	size_t queryPos = path.find("?");
	if (queryPos != std::string::npos)
		return std::make_pair(path.substr(0, queryPos), path.substr(queryPos + 1));
	return std::make_pair(path, "");
}

std::string StringUtils::GetBoundary(const std::string& contentType)
{
	size_t boundaryPos = contentType.find("boundary=");
	if (boundaryPos != std::string::npos)
	{
		std::string boundary = contentType.substr(boundaryPos + 9);
		if (!boundary.empty() && boundary[0] == '"' && boundary[boundary.length() - 1] == '"')
			boundary = boundary.substr(1, boundary.length() - 2);
		return boundary;
	}
	else
	{
		Logger::LogWarning("boundary not found");
	}
	return "";
}

std::vector<Body> StringUtils::SplitMultipartData(const Body& body, const std::string& boundary)
{
	std::vector<Body> parts;
	std::string delimiter = "--" + boundary;
	std::string endDelimiter = delimiter + "--";

	size_t pos = 0;
	while (true)
	{
		size_t startPos = body.content.find(delimiter, pos);
		if (startPos == std::string::npos)
			break;

		startPos += delimiter.length();

		size_t endPos = body.content.find(delimiter, startPos);
		if (endPos == std::string::npos)
		{
			endPos = body.content.find(endDelimiter, startPos);
			if (endPos == std::string::npos)
				endPos = body.content.size();
		}

		Ustring part = body.content.substr(startPos, endPos - startPos);
		if (!part.empty())
		{
			Body tmp;
			tmp.content = part;
			parts.push_back(tmp);
			Logger::Log("Found multipart data part of length: " + StringUtils::ToString(tmp.content.size()));
		}

		if (body.content.substr(endPos, endDelimiter.length()).toString() == endDelimiter)
			break;

		pos = endPos;
	}

	Logger::Log("Total parts found: " + StringUtils::ToString(parts.size()));
	return parts;
}

std::string StringUtils::GetScriptPath(const std::string& path)
{
	size_t queryPos = path.find("?");
	std::string scriptPath = (queryPos != std::string::npos) ? path.substr(0, queryPos) : path;
	return scriptPath;
}

char **StringUtils::GetMatrix(const std::map<std::string, std::string>& map)
{
	size_t i = 0;
	char** matrix = new char*[map.size() + 1]; 

	std::map<std::string, std::string>::const_iterator it;
	for (it = map.begin(); it != map.end(); ++it)
	{
		std::string envStr = it->first + "=" + it->second;
		matrix[i] = new char[envStr.length() + 1]; // Allocate space for the string
		std::strcpy(matrix[i], envStr.c_str());    // Copy the string into the matrix
		++i;
	}
	matrix[i] = NULL; // Null terminate the array of strings
	return matrix;
}