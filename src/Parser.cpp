# include "Parser.hpp"
# include <Logger.hpp>

// void Parser::FillServer(std::vector<Server> &Servers, const char *fileConf)
// {
// 	std::vector<Token> tokens = Lexer::GetToken(fileConf);

// 	ConfFileParser::setupServer(tokens);


// 	Servers.push_back(Server
// 	(
// 		8002,
// 		"9999",
// 		"index.html",
// 		"locahost",
// 		"web-page",
// 		"127.0.0.1",
// 		"web-page/error-pages/404.html"
// 	));

// 	Servers.push_back(Server
// 	(
// 		8003,
// 		"9999",
// 		"index.html",
// 		"locahost",
// 		"web-page",
// 		"127.0.0.1",
// 		"web-page/error-pages/404.html"
// 	));

// 	Logger::Log("finished parsing Configuration file");
// }

ConfFileParser::ConfFileParser(std::vector<Server> &Servers, const char *fileConf)
{
	this->servers = Servers;
	serverStart = 0;
	serverEnd = -1;
	locationStart = 0;
	locationEnd = -1;
	tokens = Lexer::GetToken(fileConf);
	setupServer();
}

int ConfFileParser::stringToInt(std::string str)
{
	int num = 0;
	for (size_t i = 0; i < str.size(); i++)
	{
		if (!isdigit(str[i]))
			throw std::invalid_argument("Invalid string to convert to int");
		num = num * 10 + (str[i] - '0');
	}
	return num;
}

std::vector<std::string> ConfFileParser::split(std::string str, char delim)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(str);
	while (std::getline(tokenStream, token, delim))
	{
		tokens.push_back(token);
	}
	return tokens;
}

bool ConfFileParser::isDomain(std::string str)
{
	int numDots = 0;
	std::string label;

	if (str[0] == '-' || str[str.size() - 1] == '-' || str[0] == '.' || str[str.size() - 1] == '.')
		return false;

	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == '.')
		{
			if (label.size() == 0 || label.size() > 63)
				return false;
			if (label[0] == '-' || label[label.size() - 1] == '-')
				return false;
			label = "";
			numDots++;
		}
		else if (str[i] == '-')
		{
			if (label.size() == 0 || label.size() > 63)
				return false;
			label = "";
		}
		else
			label += str[i];
	}

	if (str.size() == 0 || str.size() > 253)
		return false;
	return true;
}

bool ConfFileParser::isIp(std::string str)
{
	std::string octet;
	int numDots = 0;
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == '.')
		{
			if (octet.size() == 0 || octet.size() > 3)
				return false;
			if (stringToInt(octet) < 0 || stringToInt(octet) > 255)
				return false;
			octet = "";
			numDots++;
		}
		else if (!isdigit(str[i]))
			return false;
		else
			octet += str[i];
		if (i == str.size() - 1)
		{
			if (octet.size() == 0 || octet.size() > 3)
				return false;
			if (stringToInt(octet) < 0 || stringToInt(octet) > 255)
				return false;
		}
	}
	return true;
}

void ConfFileParser::resetValues()
{
	port = 0;
	host = "";
	locationStart = 0;
	locationEnd = -1;
	serverNames.clear();
	errorPages.clear();
}

void ConfFileParser::getServerStartEnd()
{
	int tokenSize = tokens.size();
	if (serverEnd == tokenSize - 1)
	{
		serverStart = -1;
		serverEnd = -1;
		return ;
	}
	serverStart = serverEnd + 1;

	for (size_t i = serverStart; i < tokens.size(); i++)
	{
		if (tokens[i].tokenName == "ServerEnd")
		{
			serverEnd = i;
			return;
		}
	}
}

void ConfFileParser::getLocationStartEnd()
{
	if (locationStart == -1 && locationEnd == -1)
		return ;

	if (locationEnd == -1)
	{
		for (int i = serverStart; i < serverEnd; i++)
		{
			if (tokens[i].tokenName == "location")
			{
				locationStart = i;
				for (int j = locationStart; j < serverEnd; j++)
				{
					if (tokens[j].tokenName == "LocationEnd")
					{
						locationEnd = j;
						return ;
					}
				}
			}
		}
	}
	else
	{
		for (int i = locationEnd + 1; i < serverEnd; i++)
		{
			if (tokens[i].tokenName == "location")
			{
				locationStart = i;
				for (int j = locationStart; j < serverEnd; j++)
				{
					if (tokens[j].tokenName == "LocationEnd")
					{
						locationEnd = j;
						return ;
					}
				}
			}
		}
	}
	locationStart = -1;
	locationEnd = -1;
}

void ConfFileParser::setupServer()
{
	getServerStartEnd();

	while (serverStart != -1 && serverEnd != -1)
	{
		getLocationStartEnd();
		try
		{
			while (locationStart != -1 || locationEnd != -1)
			{
				if (locationStart == -1 || locationEnd == -1)
					break;
				getLocationPath();
				getMethods();
				getRedirect();
				getRootPath("location");
				getAutoIndex("location");
				getIndex("location");
				getCgiExtension();
				getCgiPath();
				getUploadPath();
				getUploadEnable();
				getLocationStartEnd();
			}
			getPort();
			getHost();
			getServerName();
			getErrorPage();
			getClientsBodySize();
			getRootPath("server");
			getAutoIndex("server");
			resetValues();
			getServerStartEnd();
		}
		catch (const std::exception &e)
		{
			Logger::LogException(e);
			break;
		}
	}
}

void ConfFileParser::getPort()
{
	bool portFound = false;
	for (int i = serverStart; i < serverEnd; i++)
	{
		if (tokens[i].tokenName == "listen")
		{
			if (portFound)
				throw TooManyPorts();
			checkPort(tokens[i].tokenValue);
			portFound = true;
		}
	}

	if (!portFound)
		throw PortNotFound();
}

void ConfFileParser::checkPort(const std::string &portToCheck)
{
	for (size_t i = 0; i < portToCheck.size(); i++)
	{
		if (!isdigit(portToCheck[i]) || portToCheck.size() > 5)
			throw InvalidPort();
	}
	port = stringToInt(portToCheck);

	if (port < 1 || port > 65535)
		throw InvalidPort();
}

void ConfFileParser::getHost()
{
	bool hostFound = false;
	for (int i = serverStart; i < serverEnd; i++)
	{
		if (tokens[i].tokenName == "host")
		{
			if (hostFound)
				throw TooManyHosts();
			checkHost(tokens[i].tokenValue);
			hostFound = true;
		}
	}

	if (!hostFound)
		throw HostNotFound();
}

void ConfFileParser::checkHost(const std::string &host)
{

	if (isDomain(host))
	{
		this->host = host;
		return ;
	}
	if (isIp(host))
	{
		this->host = host;
		return ;
	}
	throw InvalidHost();
}

void ConfFileParser::getServerName()
{
	std::vector<std::string> names;

	for (int i = serverStart; i < serverEnd; i++)
	{
		if (tokens[i].tokenName == "server_name")
		{
			names = split(tokens[i].tokenValue, ' ');
			for (size_t j = 0; j < names.size(); j++)
			{
				checkServerName(names[j]);
			}
		}
	}
}

void ConfFileParser::checkServerName(const std::string &name)
{
	if (isDomain(name))
	{
		this->serverNames.push_back(name);
		return ;
	}
	if (isIp(name))
	{
		this->serverNames.push_back(name);
		return ;
	}
	throw InvalidServerName();
}

void ConfFileParser::getErrorPage()
{
	std::vector<std::string> errorPage;
	bool errorPageFound = false;

	for (int i = serverStart; i < serverEnd; i++)
	{
		if (tokens[i].tokenName == "error_page")
		{
			errorPageFound = true;
			errorPage = split(tokens[i].tokenValue, ' ');
			checkErrorPage(errorPage);
		}
	}

	if (!errorPageFound)
		throw ErrorPageNotFound();
}

void ConfFileParser::checkErrorPage(const std::vector<std::string> &errorPage)
{
	if (errorPage.size() != 2)
		throw InvalidErrorPage();
	if (errorPage[0].size() != 3)
		throw InvalidErrorPage();
	for (size_t i = 0; i < errorPage[0].size(); i++)
	{
		if (!isdigit(errorPage[0][i]))
			throw InvalidErrorPage();
	}

	for (size_t i = 0; i < errorPages.size(); i++)
	{
		if (errorPages[i].code == stringToInt(errorPage[0]))
			throw DuplicateErrorPage();
	}
	this->errorPages.push_back(CodePath(stringToInt(errorPage[0]), errorPage[1]));
}

void ConfFileParser::getClientsBodySize()
{
	bool clientBodySizeFound = false;
	for (int i = serverStart; i < serverEnd; i++)
	{
		if (tokens[i].tokenName == "client_max_body_size")
		{
			if (clientBodySizeFound)
				throw TooManyClientBodySize();
			clientBodySizeFound = true;
			bool lastCharIsType = false;
			char lastChar = tokens[i].tokenValue[tokens[i].tokenValue.size() - 1];

			if (lastChar == 'K' || lastChar == 'M' || lastChar == 'G')
				lastCharIsType = true;

			if (!lastCharIsType)
			{
				for (size_t j = 0; j < tokens[i].tokenValue.size(); j++)
				{
					if (!isdigit(tokens[i].tokenValue[j]))
						throw InvalidClientBodySize();
				}
				this->clientBodySize.size = tokens[i].tokenValue;
				this->clientBodySize.unit = BYTE;
			}
			else
			{
				std::string size = tokens[i].tokenValue.substr(0, tokens[i].tokenValue.size() - 1);
				if (size.size() == 0)
					throw InvalidClientBodySize();
				for (size_t j = 0; j < size.size(); j++)
				{
					if (!isdigit(size[j]))
						throw InvalidClientBodySize();
				}
				this->clientBodySize.size = size;
				if (lastChar == 'K')
					this->clientBodySize.unit = KILOBYTE;
				else if (lastChar == 'M')
					this->clientBodySize.unit = MEGABYTE;
				else if (lastChar == 'G')
					this->clientBodySize.unit = GIGABYTE;
			}
		}
	}

	if (!clientBodySizeFound)
		throw ClientBodySizeNotFound();
}

void ConfFileParser::getLocationPath()
{
	if (tokens[locationStart].tokenName != "location" || tokens[locationStart].tokenValue.size() == 0 || tokens[locationStart].tokenValue[0] != '/')
		throw InvalidLocation();
	locations.push_back(Location());
	locations[locations.size() - 1].path = tokens[locationStart].tokenValue;
}

void ConfFileParser::getMethods()
{
	std::vector<std::string> methods;
	for (int i = locationStart; i < locationEnd; i++)
	{
		if (tokens[i].tokenName == "allow_methods")
		{
			methods = split(tokens[i].tokenValue, ' ');
			for (size_t j = 0; j < methods.size(); j++)
			{
				checkMethod(methods[j]);
				locations[locations.size() - 1].methods.push_back(methods[j]);
			}
		}
	}
}

void ConfFileParser::checkMethod(const std::string &method)
{
	if (method != "GET" && method != "POST" && method != "DELETE")
		throw InvalidMethod();

}

void ConfFileParser::getRedirect()
{
	std::vector<std::string> redirect;
	bool redirectFound = false;

	for (int i = locationStart; i < locationEnd; i++)
	{
		if (tokens[i].tokenName == "return")
		{
			if (redirectFound)
				throw TooManyRedirects();
			redirectFound = true;
			redirect = split(tokens[i].tokenValue, ' ');
			checkRedirect(redirect);
		}
	}
}

void ConfFileParser::checkRedirect(const std::vector<std::string> &redirect)
{
	if (redirect.size() == 2)
	{
		for (size_t i = 0; i < redirect[0].size(); i++)
		{
			if (!isdigit(redirect[0][i]))
				throw InvalidRedirect();
		}

		locations[locations.size() - 1].redirect.code = stringToInt(redirect[0]);
		locations[locations.size() - 1].redirect.path = redirect[1];
	}
	else if (redirect.size() == 1)
	{
		locations[locations.size() - 1].redirect.code = 302;
		locations[locations.size() - 1].redirect.path = redirect[0];
	}
	else
		throw InvalidRedirect();
}

void ConfFileParser::getRootPath(const std::string &mode)
{
	if (mode == "server")
	{
		for (int i = serverStart; i < serverEnd; i++)
		{
			if (tokens[i].tokenName == "root")
			{
				if (rootPath.size() != 0)
					throw TooManyRoots();
				rootPath = tokens[i].tokenValue;
			}
		}
	}
	else if (mode == "location")
	{
		for (int i = locationStart; i < locationEnd; i++)
		{
			if (tokens[i].tokenName == "root")
			{
				if (locations[locations.size() - 1].rootPath.size() != 0)
					throw TooManyRoots();
				locations[locations.size() - 1].rootPath = tokens[i].tokenValue;
			}
		}
	}
}

void ConfFileParser::getAutoIndex(const std::string &mode)
{
	bool foundAutoIndex = false;
	if (mode == "server")
	{
		for (int i = serverStart; i < serverEnd; i++)
		{
			if (tokens[i].tokenName == "auto_index")
			{
				if (foundAutoIndex)
					throw TooManyAutoIndexes();
				foundAutoIndex = true;
				if (tokens[i].tokenValue == "on")
					autoIndex = true;
				else if (tokens[i].tokenValue == "off")
					autoIndex = false;
				else
					throw InvalidAutoIndex();
			}
		}
	}
	else if (mode == "location")
	{
		for (int i = locationStart; i < locationEnd; i++)
		{
			if (tokens[i].tokenName == "auto_index")
			{
				if (foundAutoIndex)
					throw TooManyAutoIndexes();
				foundAutoIndex = true;
				if (tokens[i].tokenValue == "on")
					locations[locations.size() - 1].autoIndex = true;
				else if (tokens[i].tokenValue == "off")
					locations[locations.size() - 1].autoIndex = false;
				else
					throw InvalidAutoIndex();
			}
		}
	}
}

void ConfFileParser::getIndex(const std::string &mode)
{
	std::vector<std::string> index;
	if (mode == "server")
	{
		for (int i = serverStart; i < serverEnd; i++)
		{
			if (tokens[i].tokenName == "index")
			{
				index = split(tokens[i].tokenValue, ' ');
				for (size_t j = 0; j < index.size(); j++)
				{
					if (index[j].size() == 0)
						throw InvalidIndex();
					this->index.push_back(index[j]);
				}
			}
		}
	}
	else if (mode == "location")
	{
		for (int i = locationStart; i < locationEnd; i++)
		{
			if (tokens[i].tokenName == "index")
			{
				index = split(tokens[i].tokenValue, ' ');
				for (size_t j = 0; j < index.size(); j++)
				{
					if (index[j].size() == 0)
						throw InvalidIndex();
					locations[locations.size() - 1].index.push_back(index[j]);
				}
			}
		}
	}
}

void ConfFileParser::getCgiExtension()
{
	bool cgiExtensionFound = false;

	for (int i = locationStart; i < locationEnd; i++)
	{
		if (tokens[i].tokenName == "cgi_extension")
		{
			if (cgiExtensionFound)
				throw TooManyCgiExtensions();
			if (tokens[i].tokenValue.size() == 0 || tokens[i].tokenValue[0] != '.')
				throw InvalidCgiExtension();
			cgiExtensionFound = true;
			locations[locations.size() - 1].cgiExtension = tokens[i].tokenValue;
		}
	}
}

void ConfFileParser::getCgiPath()
{
	bool cgiPathFound = false;

	for (int i = locationStart; i < locationEnd; i++)
	{
		if (tokens[i].tokenName == "cgi_path")
		{
			if (cgiPathFound)
				throw TooManyCgiPaths();
			cgiPathFound = true;
			locations[locations.size() - 1].cgiPath = tokens[i].tokenValue;
		}
	}
}

void ConfFileParser::getUploadPath()
{
	bool uploadPathFound = false;

	for (int i = locationStart; i < locationEnd; i++)
	{
		if (tokens[i].tokenName == "upload_path")
		{
			if (uploadPathFound)
				throw TooManyUploadPaths();
			uploadPathFound = true;
			locations[locations.size() - 1].uploadPath = tokens[i].tokenValue;
		}
	}
}

void ConfFileParser::getUploadEnable()
{
	bool uploadEnableFound = false;

	for (int i = locationStart; i < locationEnd; i++)
	{
		if (tokens[i].tokenName == "upload_enable")
		{
			if (uploadEnableFound)
				throw TooManyUploadEnables();
			uploadEnableFound = true;
			if (tokens[i].tokenValue == "on")
				locations[locations.size() - 1].uploadEnable = true;
			else if (tokens[i].tokenValue == "off")
				locations[locations.size() - 1].uploadEnable = false;
			else
				throw InvalidUploadEnable();
		}
	}
}

void ConfFileParser::setupServerConfig()
{
	servers.push_back(Server())
	servers[servers.size() - 1].serverConfig(port, host, serverNames, rootPath, index, clientBodySize, errorPages, autoIndex, locations);
}

