# include "Parser.hpp"
# include <Logger.hpp>
# include <StringUtils.hpp>
# include <NetworkUtils.hpp>

using namespace StringUtils;

void Parser::ParseConfigFile(std::vector<Server> &servers, const char *fileConf)
{
	std::vector<Lexer::Token> tokens = Lexer::GetToken(fileConf);

	Logger::Log("start parsing token...");

	int tokenSize = tokens.size();
	
	if (tokenSize == 0)
		return;

	for (size_t i = 0; i < tokens.size(); i++)
	{
		Server server;

		if (tokens[i].tokenType == startLocationContext)
		{
			while (tokens[i].tokenType == endLocationContext)
			{
				GetLocationPath(tokens[i], server);
				GetMethods(tokens[i], server);
				GetRedirect(tokens[i], server);
				GetRootPath(false, tokens[i], server);
				GetAutoIndex(false, tokens[i], server);
				GetIndex(false, tokens[i], server);
				GetCgiExtension(tokens[i], server);
				GetCgiPath(tokens[i], server);
				GetUploadPath(tokens[i], server);
				GetUploadEnable(tokens[i], server);
				// GetLocationStartEnd(tokens[i], server);

				i++;
			}
		}

		GetPort(tokens[i], server);
		GetHost(tokens[i], server);
		GetServerName(tokens[i], server);
		GetErrorPage(tokens[i], server);
		GetClientsBodySize(tokens[i], server);
		GetRootPath(true, tokens[i], server);
		GetAutoIndex(true, tokens[i], server);

		servers.push_back(server);
	}
	
	Logger::Log("parsing finished");
}

void Parser::GetServerStartEnd()
{
}

void Parser::GetLocationStartEnd()
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

void Parser::GetPort(const Token& token, Server& server)
{
	int port;

	if (token.tokenName == "listen")
	{
		if (server.serverConfig.serverPort == 0)
		{
			port = StrintToInt(token.tokenValue);

			if (port < 1 || port > 65535)
				throw InvalidPort();
				
			server.serverConfig.serverPort = port;
		}
		else
			throw TooManyPorts();
	}
}

void Parser::GetHost(const Token& token, Server& server)
{
	if (token.tokenName == "host")
	{
		if (!server.serverConfig.host.empty())
			throw TooManyHosts();
		
		if (NetworkUtils::IsDomain(token.tokenValue) || NetworkUtils::IsValidateIp(token.tokenValue))
			server.serverConfig.host = token.tokenValue;
		else
			throw InvalidHost();
	}
}

void Parser::GetServerName(const Token& token, Server& server)
{
	std::vector<std::string> names;

	if (token.tokenName == "server_name")
	{
		names = StringUtils::Split(token.tokenValue, ' ');

		for (size_t j = 0; j < names.size(); j++)
		{
			if (NetworkUtils::IsDomain(names[j]) || NetworkUtils::IsValidateIp(names[j]))
				server.serverConfig.serverNames.push_back(names[j]);
			else
				throw InvalidServerName();
		}
	}
}

void Parser::GetErrorPage(const Token& token, Server& server)
{
	if (token.tokenName == "error_page")
	{
		std::vector<std::string> errorPage;
		errorPage = StringUtils::Split(token.tokenValue, ' ');

		CheckErrorPage(errorPage, server);
	}
}

void Parser::CheckErrorPage(const std::vector<std::string> &errorPage,  Server& server)
{
	if (errorPage.size() != 2 || errorPage[0].size() != 3)
		throw InvalidErrorPage();

	if (!StringUtils::IsAllDigit(errorPage[0]))
		throw InvalidErrorPage();

	server.serverConfig.errorPage.push_back(CodePath(StrintToInt(errorPage[0]), errorPage[1]));
}

void Parser::GetClientsBodySize(const Token& token, Server& server)
{
	if (token.tokenName == "client_max_body_size")
	{
		char lastChar = token.tokenValue[token.tokenValue.size() - 1];

		if (!(lastChar == 'K' || lastChar == 'M' || lastChar == 'G'))
		{
			if(!StringUtils::IsAllDigit(token.tokenValue))
					throw InvalidClientBodySize();

			server.serverConfig.clientMaxBody.size = token.tokenValue;
			server.serverConfig.clientMaxBody.unit =  BYTE;
		}
		else
		{
			std::string size = token.tokenValue.substr(0, token.tokenValue.size() - 1);
			
			if (size.size() == 0)
				throw InvalidClientBodySize();
			
			if(!StringUtils::IsAllDigit(size))
				throw InvalidClientBodySize();
			
			server.serverConfig.clientMaxBody.size = size;
			
			if (lastChar == 'K')
				server.serverConfig.clientMaxBody.unit = KILOBYTE;
			else if (lastChar == 'M')
				server.serverConfig.clientMaxBody.unit = MEGABYTE;
			else if (lastChar == 'G')
				server.serverConfig.clientMaxBody.unit = GIGABYTE;
		}
	}
}

void Parser::GetLocationPath(const Token& token, Server& server)
{
	(void)token;
	(void)server;
	// if (tokens[locationStart].tokenName != "location" || tokens[locationStart].tokenValue.size() == 0 || tokens[locationStart].tokenValue[0] != '/')
	// 	throw InvalidLocation();
	// locations.push_back(Location());
	// locations[locations.size() - 1].path = tokens[locationStart].tokenValue;
}

void Parser::GetMethods(const Token& token, Server& server)
{
	(void)token;
	(void)server;
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

void Parser::CheckMethod(const std::string &method)
{
	(void)method;
	// if (method != "GET" && method != "POST" && method != "DELETE")
	// 	throw InvalidMethod();

}

void Parser::GetRedirect(const Token& token, Server& server)
{
	(void)token;
	(void)server;
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

void Parser::CheckRedirect(const std::vector<std::string> &redirect)
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

void Parser::GetRootPath(bool isServer,const Token& token, Server& server)
{
	if (isServer)
	{
		if (token.tokenName == "root")
		{
			if(server.serverConfig.serverRoot != "")
				throw TooManyRoots();

			server.serverConfig.serverRoot = token.tokenValue;
		}
	}
	else
	{
		if (token.tokenName == "root")
		{
			for (size_t i = 0; i < server.serverConfig.locations.size(); i++)
			{
				if (server.serverConfig.locations[i].rootPath.size() != 0)
					throw TooManyRoots();

				server.serverConfig.locations[i].rootPath = token.tokenValue;
			}
		}
	}
}

void Parser::GetAutoIndex(bool isServer, const Token& token, Server& server)
{
	(void)isServer;
	(void)token;
	(void)server;
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

void Parser::GetIndex(bool isServer, const Token& token, Server& server)
{
	(void)isServer;
	(void)token;
	(void)server;
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

void Parser::GetCgiExtension(const Token& token, Server& server)
{
	(void)token;
	(void)server;
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

void Parser::GetCgiPath(const Token& token, Server& server)
{
	(void)token;
	(void)server;
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

void Parser::GetUploadPath(const Token& token, Server& server)
{
	(void)token;
	(void)server;
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

void Parser::GetUploadEnable(const Token& token, Server& server)
{
	(void)token;
	(void)server;
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

void Parser::SetupServerConfig(const Token& token, Server& server)
{
	(void)token;
	(void)server;
	// servers.push_back(Server());
	// servers[servers.size() - 1].serverConfig(port, host, serverNames, rootPath, index, clientBodySize, errorPages, autoIndex, locations);
}

