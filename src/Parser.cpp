# include "Parser.hpp"
# include <Logger.hpp>
# include <Lexer.hpp>
# include <Utils.hpp>

using namespace StringUtils;

void Parser::ParseConfigFile(std::vector<Server> &servers, const char *fileConf)
{
	(void)fileConf;
	(void)servers;

	// int serverStart = 0;
	// int serverEnd = -1;
	// int locationStart = 0;
	// int locationEnd = -1;

	// std::vector<Lexer::Token> tokens = Lexer::GetToken(fileConf);

	// int tokenSize = tokens.size();

	// if (serverEnd == tokenSize - 1)
	// {
	// 	serverStart = -1;
	// 	serverEnd = -1;
	// 	return ;
	// }
	// serverStart = serverEnd + 1;

	// for (size_t i = serverStart; i < tokens.size(); i++)
	// {
	// 	if (tokens[i].tokenName == "ServerEnd")
	// 	{
	// 		serverEnd = i;
	// 		return;
	// 	}
	// }

	// while (serverStart != -1 && serverEnd != -1)
	// {
	// 	getLocationStartEnd();
	// 	try
	// 	{
	// 		while (locationStart != -1 || locationEnd != -1)
	// 		{
	// 			if (locationStart == -1 || locationEnd == -1)
	// 				break;
	// 			getLocationPath();
	// 			getMethods();
	// 			getRedirect();
	// 			getRootPath("location");
	// 			getAutoIndex("location");
	// 			getIndex("location");
	// 			getCgiExtension();
	// 			getCgiPath();
	// 			getUploadPath();
	// 			getUploadEnable();
	// 			getLocationStartEnd();
	// 		}
	// 		getPort();
	// 		getHost();
	// 		getServerName();
	// 		getErrorPage();
	// 		getClientsBodySize();
	// 		getRootPath("server");
	// 		getAutoIndex("server");
	// 		resetValues();
	// 		getServerStartEnd();
	// 	}
	// 	catch (const std::exception &e)
	// 	{
	// 		Logger::LogException(e);
	// 		break;
	// 	}
	// }

	// setupServer();
}

void Parser::resetValues()
{
	// port = 0;
	// host = "";
	// locationStart = 0;
	// locationEnd = -1;
	// serverNames.clear();
	// errorPages.clear();
}

void Parser::getServerStartEnd()
{
}

void Parser::getLocationStartEnd()
{
	// if (locationStart == -1 && locationEnd == -1)
	// 	return ;

	// if (locationEnd == -1)
	// {
	// 	for (int i = serverStart; i < serverEnd; i++)
	// 	{
	// 		if (tokens[i].tokenName == "location")
	// 		{
	// 			locationStart = i;
	// 			for (int j = locationStart; j < serverEnd; j++)
	// 			{
	// 				if (tokens[j].tokenName == "LocationEnd")
	// 				{
	// 					locationEnd = j;
	// 					return ;
	// 				}
	// 			}
	// 		}
	// 	}
	// }
	// else
	// {
	// 	for (int i = locationEnd + 1; i < serverEnd; i++)
	// 	{
	// 		if (tokens[i].tokenName == "location")
	// 		{
	// 			locationStart = i;
	// 			for (int j = locationStart; j < serverEnd; j++)
	// 			{
	// 				if (tokens[j].tokenName == "LocationEnd")
	// 				{
	// 					locationEnd = j;
	// 					return ;
	// 				}
	// 			}
	// 		}
	// 	}
	// }
	// locationStart = -1;
	// locationEnd = -1;
}

void Parser::getPort()
{
	// bool portFound = false;
	// for (int i = serverStart; i < serverEnd; i++)
	// {
	// 	if (tokens[i].tokenName == "listen")
	// 	{
	// 		if (portFound)
	// 			throw TooManyPorts();
	// 		checkPort(tokens[i].tokenValue);
	// 		portFound = true;
	// 	}
	// }

	// if (!portFound)
	// 	throw PortNotFound();
}

void Parser::checkPort(const std::string &portToCheck)
{
	(void)portToCheck;
	// for (size_t i = 0; i < portToCheck.size(); i++)
	// {
	// 	if (!isdigit(portToCheck[i]) || portToCheck.size() > 5)
	// 		throw InvalidPort();
	// }
	// port = StrintToInt(portToCheck);

	// if (port < 1 || port > 65535)
	// 	throw InvalidPort();
}

void Parser::getHost()
{
	// bool hostFound = false;
	// for (int i = serverStart; i < serverEnd; i++)
	// {
	// 	if (tokens[i].tokenName == "host")
	// 	{
	// 		if (hostFound)
	// 			throw TooManyHosts();
	// 		checkHost(tokens[i].tokenValue);
	// 		hostFound = true;
	// 	}
	// }

	// if (!hostFound)
	// 	throw HostNotFound();
}

void Parser::checkHost(const std::string &host)
{
	(void)host;
	// if (isDomain(host))
	// {
	// 	this->host = host;
	// 	return ;
	// }
	// if (isIp(host))
	// {
	// 	this->host = host;
	// 	return ;
	// }
	// throw InvalidHost();
}

void Parser::getServerName()
{
	// std::vector<std::string> names;

	// for (int i = serverStart; i < serverEnd; i++)
	// {
	// 	if (tokens[i].tokenName == "server_name")
	// 	{
	// 		names = split(tokens[i].tokenValue, ' ');
	// 		for (size_t j = 0; j < names.size(); j++)
	// 		{
	// 			checkServerName(names[j]);
	// 		}
	// 	}
	// }
}

void Parser::checkServerName(const std::string &name)
{
	(void)name;
	// if (isDomain(name))
	// {
	// 	this->serverNames.push_back(name);
	// 	return ;
	// }
	// if (isIp(name))
	// {
	// 	this->serverNames.push_back(name);
	// 	return ;
	// }
	// throw InvalidServerName();
}

void Parser::getErrorPage()
{
	// std::vector<std::string> errorPage;
	// bool errorPageFound = false;

	// for (int i = serverStart; i < serverEnd; i++)
	// {
	// 	if (tokens[i].tokenName == "error_page")
	// 	{
	// 		errorPageFound = true;
	// 		errorPage = split(tokens[i].tokenValue, ' ');
	// 		checkErrorPage(errorPage);
	// 	}
	// }

	// if (!errorPageFound)
	// 	throw ErrorPageNotFound();
}

void Parser::checkErrorPage(const std::vector<std::string> &errorPage)
{
	(void)errorPage;
	// if (errorPage.size() != 2)
	// 	throw InvalidErrorPage();
	// if (errorPage[0].size() != 3)
	// 	throw InvalidErrorPage();
	// for (size_t i = 0; i < errorPage[0].size(); i++)
	// {
	// 	if (!isdigit(errorPage[0][i]))
	// 		throw InvalidErrorPage();
	// }

	// for (size_t i = 0; i < errorPages.size(); i++)
	// {
	// 	if (errorPages[i].code == StrintToInt(errorPage[0]))
	// 		throw DuplicateErrorPage();
	// }
	// this->errorPages.push_back(CodePath(StrintToInt(errorPage[0]), errorPage[1]));
}

void Parser::getClientsBodySize()
{
	// bool clientBodySizeFound = false;
	// for (int i = serverStart; i < serverEnd; i++)
	// {
	// 	if (tokens[i].tokenName == "client_max_body_size")
	// 	{
	// 		if (clientBodySizeFound)
	// 			throw TooManyClientBodySize();
	// 		clientBodySizeFound = true;
	// 		bool lastCharIsType = false;
	// 		char lastChar = tokens[i].tokenValue[tokens[i].tokenValue.size() - 1];

	// 		if (lastChar == 'K' || lastChar == 'M' || lastChar == 'G')
	// 			lastCharIsType = true;

	// 		if (!lastCharIsType)
	// 		{
	// 			for (size_t j = 0; j < tokens[i].tokenValue.size(); j++)
	// 			{
	// 				if (!isdigit(tokens[i].tokenValue[j]))
	// 					throw InvalidClientBodySize();
	// 			}
	// 			this->clientBodySize.size = tokens[i].tokenValue;
	// 			this->clientBodySize.unit = BYTE;
	// 		}
	// 		else
	// 		{
	// 			std::string size = tokens[i].tokenValue.substr(0, tokens[i].tokenValue.size() - 1);
	// 			if (size.size() == 0)
	// 				throw InvalidClientBodySize();
	// 			for (size_t j = 0; j < size.size(); j++)
	// 			{
	// 				if (!isdigit(size[j]))
	// 					throw InvalidClientBodySize();
	// 			}
	// 			this->clientBodySize.size = size;
	// 			if (lastChar == 'K')
	// 				this->clientBodySize.unit = KILOBYTE;
	// 			else if (lastChar == 'M')
	// 				this->clientBodySize.unit = MEGABYTE;
	// 			else if (lastChar == 'G')
	// 				this->clientBodySize.unit = GIGABYTE;
	// 		}
	// 	}
	// }

	// if (!clientBodySizeFound)
	// 	throw ClientBodySizeNotFound();
}

void Parser::getLocationPath()
{
	// if (tokens[locationStart].tokenName != "location" || tokens[locationStart].tokenValue.size() == 0 || tokens[locationStart].tokenValue[0] != '/')
	// 	throw InvalidLocation();
	// locations.push_back(Location());
	// locations[locations.size() - 1].path = tokens[locationStart].tokenValue;
}

void Parser::getMethods()
{
	// std::vector<std::string> methods;
	// for (int i = locationStart; i < locationEnd; i++)
	// {
	// 	if (tokens[i].tokenName == "allow_methods")
	// 	{
	// 		methods = split(tokens[i].tokenValue, ' ');
	// 		for (size_t j = 0; j < methods.size(); j++)
	// 		{
	// 			checkMethod(methods[j]);
	// 			locations[locations.size() - 1].methods.push_back(methods[j]);
	// 		}
	// 	}
	// }
}

void Parser::checkMethod(const std::string &method)
{
	(void)method;
	// if (method != "GET" && method != "POST" && method != "DELETE")
	// 	throw InvalidMethod();

}

void Parser::getRedirect()
{
	// std::vector<std::string> redirect;
	// bool redirectFound = false;

	// for (int i = locationStart; i < locationEnd; i++)
	// {
	// 	if (tokens[i].tokenName == "return")
	// 	{
	// 		if (redirectFound)
	// 			throw TooManyRedirects();
	// 		redirectFound = true;
	// 		redirect = split(tokens[i].tokenValue, ' ');
	// 		checkRedirect(redirect);
	// 	}
	// }
}

void Parser::checkRedirect(const std::vector<std::string> &redirect)
{
	(void)redirect;
	// if (redirect.size() == 2)
	// {
	// 	for (size_t i = 0; i < redirect[0].size(); i++)
	// 	{
	// 		if (!isdigit(redirect[0][i]))
	// 			throw InvalidRedirect();
	// 	}

	// 	locations[locations.size() - 1].redirect.code = StrintToInt(redirect[0]);
	// 	locations[locations.size() - 1].redirect.path = redirect[1];
	// }
	// else if (redirect.size() == 1)
	// {
	// 	locations[locations.size() - 1].redirect.code = 302;
	// 	locations[locations.size() - 1].redirect.path = redirect[0];
	// }
	// else
	// 	throw InvalidRedirect();
}

void Parser::getRootPath(const std::string &mode)
{
	(void)mode;
	// if (mode == "server")
	// {
	// 	for (int i = serverStart; i < serverEnd; i++)
	// 	{
	// 		if (tokens[i].tokenName == "root")
	// 		{
	// 			if (rootPath.size() != 0)
	// 				throw TooManyRoots();
	// 			rootPath = tokens[i].tokenValue;
	// 		}
	// 	}
	// }
	// else if (mode == "location")
	// {
	// 	for (int i = locationStart; i < locationEnd; i++)
	// 	{
	// 		if (tokens[i].tokenName == "root")
	// 		{
	// 			if (locations[locations.size() - 1].rootPath.size() != 0)
	// 				throw TooManyRoots();
	// 			locations[locations.size() - 1].rootPath = tokens[i].tokenValue;
	// 		}
	// 	}
	// }
}

void Parser::getAutoIndex(const std::string &mode)
{
	(void)mode;
	// bool foundAutoIndex = false;
	// if (mode == "server")
	// {
	// 	for (int i = serverStart; i < serverEnd; i++)
	// 	{
	// 		if (tokens[i].tokenName == "auto_index")
	// 		{
	// 			if (foundAutoIndex)
	// 				throw TooManyAutoIndexes();
	// 			foundAutoIndex = true;
	// 			if (tokens[i].tokenValue == "on")
	// 				autoIndex = true;
	// 			else if (tokens[i].tokenValue == "off")
	// 				autoIndex = false;
	// 			else
	// 				throw InvalidAutoIndex();
	// 		}
	// 	}
	// }
	// else if (mode == "location")
	// {
	// 	for (int i = locationStart; i < locationEnd; i++)
	// 	{
	// 		if (tokens[i].tokenName == "auto_index")
	// 		{
	// 			if (foundAutoIndex)
	// 				throw TooManyAutoIndexes();
	// 			foundAutoIndex = true;
	// 			if (tokens[i].tokenValue == "on")
	// 				locations[locations.size() - 1].autoIndex = true;
	// 			else if (tokens[i].tokenValue == "off")
	// 				locations[locations.size() - 1].autoIndex = false;
	// 			else
	// 				throw InvalidAutoIndex();
	// 		}
	// 	}
	// }
}

void Parser::getIndex(const std::string &mode)
{
	(void)mode;
	// std::vector<std::string> index;
	// if (mode == "server")
	// {
	// 	for (int i = serverStart; i < serverEnd; i++)
	// 	{
	// 		if (tokens[i].tokenName == "index")
	// 		{
	// 			index = split(tokens[i].tokenValue, ' ');
	// 			for (size_t j = 0; j < index.size(); j++)
	// 			{
	// 				if (index[j].size() == 0)
	// 					throw InvalidIndex();
	// 				this->index.push_back(index[j]);
	// 			}
	// 		}
	// 	}
	// }
	// else if (mode == "location")
	// {
	// 	for (int i = locationStart; i < locationEnd; i++)
	// 	{
	// 		if (tokens[i].tokenName == "index")
	// 		{
	// 			index = split(tokens[i].tokenValue, ' ');
	// 			for (size_t j = 0; j < index.size(); j++)
	// 			{
	// 				if (index[j].size() == 0)
	// 					throw InvalidIndex();
	// 				locations[locations.size() - 1].index.push_back(index[j]);
	// 			}
	// 		}
	// 	}
	// }
}

void Parser::getCgiExtension()
{
	// bool cgiExtensionFound = false;

	// for (int i = locationStart; i < locationEnd; i++)
	// {
	// 	if (tokens[i].tokenName == "cgi_extension")
	// 	{
	// 		if (cgiExtensionFound)
	// 			throw TooManyCgiExtensions();
	// 		if (tokens[i].tokenValue.size() == 0 || tokens[i].tokenValue[0] != '.')
	// 			throw InvalidCgiExtension();
	// 		cgiExtensionFound = true;
	// 		locations[locations.size() - 1].cgiExtension = tokens[i].tokenValue;
	// 	}
	// }
}

void Parser::getCgiPath()
{
	// bool cgiPathFound = false;

	// for (int i = locationStart; i < locationEnd; i++)
	// {
	// 	if (tokens[i].tokenName == "cgi_path")
	// 	{
	// 		if (cgiPathFound)
	// 			throw TooManyCgiPaths();
	// 		cgiPathFound = true;
	// 		locations[locations.size() - 1].cgiPath = tokens[i].tokenValue;
	// 	}
	// }
}

void Parser::getUploadPath()
{
	// bool uploadPathFound = false;

	// for (int i = locationStart; i < locationEnd; i++)
	// {
	// 	if (tokens[i].tokenName == "upload_path")
	// 	{
	// 		if (uploadPathFound)
	// 			throw TooManyUploadPaths();
	// 		uploadPathFound = true;
	// 		locations[locations.size() - 1].uploadPath = tokens[i].tokenValue;
	// 	}
	// }
}

void Parser::getUploadEnable()
{
	// bool uploadEnableFound = false;

	// for (int i = locationStart; i < locationEnd; i++)
	// {
	// 	if (tokens[i].tokenName == "upload_enable")
	// 	{
	// 		if (uploadEnableFound)
	// 			throw TooManyUploadEnables();
	// 		uploadEnableFound = true;
	// 		if (tokens[i].tokenValue == "on")
	// 			locations[locations.size() - 1].uploadEnable = true;
	// 		else if (tokens[i].tokenValue == "off")
	// 			locations[locations.size() - 1].uploadEnable = false;
	// 		else
	// 			throw InvalidUploadEnable();
	// 	}
	// }
}

void Parser::setupServerConfig()
{
	// servers.push_back(Server());
	// servers[servers.size() - 1].serverConfig(port, host, serverNames, rootPath, index, clientBodySize, errorPages, autoIndex, locations);
}

