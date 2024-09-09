#include <StringUtils.hpp>
#include <sstream>
#include <cstring>
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

size_t StringUtils::StringToSizeT(const std::string& str)
{
	size_t number;

	for (size_t i = 0; i < str.size(); i++)
	{
		if (!isdigit(str[i]))
			throw std::invalid_argument("Invalid string to convert to size_t");
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

std::string StringUtils::Trim(const std::string &str)
{
	size_t first = str.find_first_not_of(" \t\n\r");
	if (first == std::string::npos)
		return "";
	size_t last = str.find_last_not_of(" \t\n\r");
	return str.substr(first, (last - first + 1));
}

std::pair<std::string, std::string> StringUtils::SplitPathAndQuery(const std::string &path)
{
	size_t queryPos = path.find("?");
	if (queryPos != std::string::npos)
		return std::make_pair(path.substr(0, queryPos), path.substr(queryPos + 1));
	return std::make_pair(path, "");
}

std::string StringUtils::ExtractFileContent(const std::string& part)
{
	size_t headerEnd = part.find("Content-Type:");
	// if (headerEnd == std::string::npos)
	// {
	// 	Logger::LogError("Failed to find Content-Type header in multipart data");
	// 	return "";
	// }

	size_t contentStart = part.find("\n", headerEnd);
	if (contentStart == std::string::npos)
	{
		Logger::LogError("Failed to find start of content in multipart data");
		return "";
	}

	contentStart += 1;

	std::string content = part.substr(contentStart);

	content.erase(0, content.find_first_not_of("\t\r\n"));

	return content;
}

std::string StringUtils::ExtractFilename(const std::string &part)
{
	size_t filenamePos = part.find("filename=\"");
	if (filenamePos != std::string::npos)
	{
		size_t filenameStart = filenamePos + 10;
		size_t filenameEnd = part.find("\"", filenameStart);
		if (filenameEnd != std::string::npos)
		{
			std::string filename = part.substr(filenameStart, filenameEnd - filenameStart);
			Logger::Log("Extracted filename: " + filename);
			return filename;
		}
	}
	Logger::LogError("Failed to extract filename from multipart data part");
	return "";
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
	return "";
}

std::vector<std::string> StringUtils::SplitMultipartData(const std::string& data, const std::string& boundary)
{
	std::vector<std::string> parts;
	std::string delimiter = "--" + boundary;
	std::string endDelimiter = delimiter + "--";

	size_t pos = 0;
	while (true)
	{
		size_t startPos = data.find(delimiter, pos);
		if (startPos == std::string::npos)
			break;

		startPos += delimiter.length();

		size_t endPos = data.find(delimiter, startPos);
		if (endPos == std::string::npos)
		{
			endPos = data.find(endDelimiter, startPos);
			if (endPos == std::string::npos)
				endPos = data.length();
		}

		std::string part = data.substr(startPos, endPos - startPos);
		part = StringUtils::Trim(part);
		if (!part.empty())
		{
			parts.push_back(part);
			Logger::Log("Found multipart data part of length: " + StringUtils::ToString(part.length()));
		}

		if (data.substr(endPos, endDelimiter.length()) == endDelimiter)
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
