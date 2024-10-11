#include "Body.hpp"
#include <string>
#include <Logger.hpp>

Body &Body::operator=(const Ustring &body) 
{
    this->content = body;
	return *this;
}

Body &Body::operator=(const std::string &body)
{
    this->content = body;
	return *this;
}

std::string Body::GetFileName() const
{
	size_t filenamePos = content.find("filename=\"");
	if (filenamePos != std::string::npos)
	{
		size_t filenameStart = filenamePos + 10;
		size_t filenameEnd = content.find("\"", filenameStart);
		if (filenameEnd != std::string::npos)
		{
			std::string filename = content.substr(filenameStart, filenameEnd - filenameStart).toString();
			Logger::Log("Extracted filename: " + filename);
			return filename;
		}
	}
	Logger::LogError("Failed to extract filename from multipart data part");
	return "";
}

std::string Body::GetBoundary() const
{
	size_t boundaryPos = content.find("boundary=");
	if (boundaryPos != std::string::npos)
	{
		std::string boundary = content.substr(boundaryPos + 9).toString();
		if (!boundary.empty() && boundary[0] == '"' && boundary[boundary.length() - 1] == '"')
			boundary = boundary.substr(1, boundary.length() - 2);
		return boundary;
	}
	return "";
}

bool Body::IsBinary() const
{
	size_t startType = content.find("Content-Type:");
	if (startType == std::string::npos)
	{
		Logger::LogError("Failed to find start startType");
		return false;
	}
	size_t endString = content.find("\n", startType);
	if (endString == std::string::npos)
	{
		Logger::LogError("Failed to find endString");
		return false;
	}
	
	std::string type = content.substr(startType + 14,endString - (startType + 14)).toString();
	std::cout << "type" << type << std::endl;

	if (type.find("text/plain") != std::string::npos)
		return false;

	return true;
}

size_t Body::size() const
{
    return this->content.size();
}

Ustring Body::GetFileContent() const
{
	Ustring tmpContent = content;

	size_t headerEnd = tmpContent.find("Content-Type:");
	if (headerEnd == std::string::npos)
	{
		Logger::LogError("Failed to find start of content in multipart data");
		return Ustring();
	}

	size_t contentStart = tmpContent.find("\n", headerEnd);
	if (contentStart == std::string::npos)
	{
		Logger::LogError("Failed to find start of content in multipart data");
		return Ustring();
	}

	contentStart += 1;

	Ustring tmp = tmpContent.substr(contentStart);

	tmp.erase(0, tmp.find_first_not_of("\t\r\n"));

	return tmp;
}

std::ostream &operator<<(std::ostream &os, const Body &msg)
{
    for (size_t i = 0; i < msg.content.size(); ++i)
        os << (msg.content[i]);
    return os;
}
