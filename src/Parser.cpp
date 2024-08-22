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

		if (tokens[i].tokenType == startServerContext)
		{
			while (tokens[i].tokenType != endServerContext)
			{
				if (tokens[i].tokenType == startLocationContext)
				{
					while (tokens[i].tokenType != endLocationContext)
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

						i++;
					}
				}

				GetPort(tokens[i], server);
				GetHost(tokens[i], server);
				GetServerName(tokens[i], server);
				GetIndex(true, tokens[i], server);
				GetErrorPage(tokens[i], server);
				GetClientsBodySize(tokens[i], server);
				GetRootPath(true, tokens[i], server);
				GetAutoIndex(true, tokens[i], server);

				i++;
			}
			CheckServerConfig(server);
			servers.push_back(server);
		}

	}
	CheckMultiplePorts(servers);

	Logger::Log("parsing finished");
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
		if (server.serverConfig.clientMaxBody.size.size() != 0)
			throw TooManyClientBodySize();

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
	if (token.tokenName == "location")
	{
		if (token.tokenValue.size() == 0 || token.tokenValue[0] != '/')
			throw InvalidLocation();

		Location location;

		location.path = token.tokenValue;
		server.serverConfig.locations.push_back(location);
	}
}

void Parser::GetMethods(const Token& token, Server& server)
{

	if (token.tokenName == "allow_methods")
	{
		std::vector<std::string> methods;

		methods = StringUtils::Split(token.tokenValue, ' ');

		for (size_t i = 0; i < methods.size(); i++)
		{
			if (methods[i] != "GET" && methods[i] != "POST" && methods[i] != "DELETE")
				throw InvalidMethod();

			server.serverConfig.locations[server.serverConfig.locations.size() - 1].methods.push_back(methods[i]);
		}
	}
}

void Parser::GetRedirect(const Token& token, Server& server)
{
	if (token.tokenName == "return")
	{
		std::vector<std::string> redirect;

		if (server.serverConfig.locations[server.serverConfig.locations.size() - 1].redirect.code != 0)
			throw TooManyRedirects();

		redirect = StringUtils::Split(token.tokenValue, ' ');

		if (redirect.size() == 2)
		{
			if (!StringUtils::IsAllDigit(redirect[0]))
					throw InvalidRedirect();

			server.serverConfig.locations[server.serverConfig.locations.size() - 1].redirect.code = StrintToInt(redirect[0]);
			server.serverConfig.locations[server.serverConfig.locations.size() - 1].redirect.path = redirect[1];
		}
		else if (redirect.size() == 1)
		{
			server.serverConfig.locations[server.serverConfig.locations.size() - 1].redirect.code = 302;
			server.serverConfig.locations[server.serverConfig.locations.size() - 1].redirect.path = redirect[0];
		}
		else
			throw InvalidRedirect();
		}
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
	if (isServer)
	{
		if (token.tokenName == "auto_index")
		{
			if (server.serverConfig.isInitAutoindex)
				throw TooManyAutoIndexes();

			server.serverConfig.isInitAutoindex = true;
			if (token.tokenValue == "on")
				server.serverConfig.autoIndex = true;
			else if (token.tokenValue == "off")
				server.serverConfig.autoIndex = false;
			else
				throw InvalidAutoIndex();
		}
	}
	else
	{
		if (token.tokenName == "auto_index")
		{
			if (server.serverConfig.isInitAutoindex)
				throw TooManyAutoIndexes();

			server.serverConfig.isInitAutoindex = true;
			if (token.tokenValue == "on")
				server.serverConfig.locations[server.serverConfig.locations.size() - 1].autoIndex = true;
			else if (token.tokenValue == "off")
				server.serverConfig.locations[server.serverConfig.locations.size() - 1].autoIndex = false;
			else
				throw InvalidAutoIndex();
		}
	}
}

void Parser::GetIndex(bool isServer, const Token& token, Server& server)
{
	std::vector<std::string> index;
	if (isServer)
	{
		if (token.tokenName == "index")
		{
			index = StringUtils::Split(token.tokenValue, ' ');
			for (size_t j = 0; j < index.size(); j++)
			{
				if (index[j].size() == 0)
					throw InvalidIndex();
				server.serverConfig.indexPages.push_back(index[j]);
			}
		}
	}
	else
	{
		if (token.tokenName == "index")
		{
			index = StringUtils::Split(token.tokenValue, ' ');
			for (size_t j = 0; j < index.size(); j++)
			{
				if (index[j].size() == 0)
					throw InvalidIndex();
				server.serverConfig.locations[server.serverConfig.locations.size() - 1].index.push_back(index[j]);
			}
		}
	}
}

void Parser::GetCgiExtension(const Token& token, Server& server)
{
	if (token.tokenName == "cgi_extension")
	{
		if(!server.serverConfig.locations[server.serverConfig.locations.size() - 1].cgiExtension.empty())
			throw TooManyCgiExtensions();

		if (token.tokenValue.size() == 0 || token.tokenValue[0] != '.')
			throw InvalidCgiExtension();

		server.serverConfig.locations[server.serverConfig.locations.size() - 1].cgiExtension = token.tokenValue;
	}
}

void Parser::GetCgiPath(const Token& token, Server& server)
{
	if (token.tokenName == "cgi_path")
	{
		if(!server.serverConfig.locations[server.serverConfig.locations.size() - 1].cgiPath.empty())
			throw TooManyCgiPaths();
		server.serverConfig.locations[server.serverConfig.locations.size() - 1].cgiPath = token.tokenValue;
	}
}

void Parser::GetUploadPath(const Token& token, Server& server)
{
	if (token.tokenName == "upload_path")
	{
		if(!server.serverConfig.locations[server.serverConfig.locations.size() - 1].uploadPath.empty())
			throw TooManyCgiPaths();
		server.serverConfig.locations[server.serverConfig.locations.size() - 1].uploadPath = token.tokenValue;
	}
}

void Parser::GetUploadEnable(const Token& token, Server& server)
{
	if (token.tokenName == "upload_enable")
	{
		if(!server.serverConfig.locations[server.serverConfig.locations.size() - 1].uploadEnableFind == false)
			throw TooManyUploadEnables();

		server.serverConfig.locations[server.serverConfig.locations.size() - 1].uploadEnableFind = true;
		if (token.tokenValue == "on")
			server.serverConfig.locations[server.serverConfig.locations.size() - 1].uploadEnable = true;
		else if (token.tokenValue == "off")
			server.serverConfig.locations[server.serverConfig.locations.size() - 1].uploadEnable = false;
		else
			throw InvalidUploadEnable();
	}
}

void Parser::CheckServerConfig(const Server& server)
{
	if (server.serverConfig.serverPort == 0)
		throw PortNotFound();

	if (server.serverConfig.host.empty())
		throw HostNotFound();

	if (server.serverConfig.clientMaxBody.size.empty())
		throw ClientBodySizeNotFound();

	if (server.serverConfig.errorPage.size() == 0)
		throw ErrorPageNotFound();

}

void Parser::CheckMultiplePorts(const std::vector<Server> &servers)
{
	std::vector<int> ports;

	for (size_t i = 0; i < servers.size(); i++)
	{
		ports.push_back(servers[i].serverConfig.serverPort);
	}

	for (size_t i = 0; i < ports.size(); i++)
	{
		for (size_t j = i + 1; j < ports.size(); j++)
		{
			if (ports[i] == ports[j])
				throw TooManyPorts();
		}
	}

}
