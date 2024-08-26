#include "Parser.hpp"
#include <Logger.hpp>
#include <StringUtils.hpp>
#include <NetworkUtils.hpp>

using namespace StringUtils;

void Parser::ParseConfigFile(std::vector<Server> &servers, const char *fileConf)
{
	std::vector<Lexer::Token> tokens = Lexer::GetToken(fileConf);

	Logger::Log("start parsing token...");

	if (tokens.size() == 0)
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
			server.serverConfig.CheckServerConfig();

			servers.push_back(server);
		}

	}

	Logger::Log("parsing finished");
}

void Parser::GetPort(const Token& token, Server& server)
{
	if (token.tokenName == "listen")
	{
		if (server.serverConfig.serverPort.port == 0)
			server.serverConfig.serverPort.InitPort(token.tokenValue);
		else
			throw std::invalid_argument("Too many ports");
	}
}

void Parser::GetHost(const Token& token, Server& server)
{
	if (token.tokenName == "host")
	{
		if (!server.serverConfig.host.empty())
			throw std::invalid_argument("Too many Host");

		if (NetworkUtils::IsValidateIp(token.tokenValue))
			server.serverConfig.socketIp = token.tokenValue;
		if (NetworkUtils::IsDomain(token.tokenValue))
			server.serverConfig.host = token.tokenValue;
		else
			throw std::invalid_argument("Invalid host");
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
			if (NetworkUtils::IsDomain(names[j]))
				server.serverConfig.serverNames.push_back(names[j]);
			else
				throw std::invalid_argument("Invalid server name");
		}
	}
}

void Parser::GetErrorPage(const Token& token, Server& server)
{
	if (token.tokenName == "error_page")
	{
		std::vector<std::string> errorPage = StringUtils::Split(token.tokenValue, ' ');

		if (errorPage.size() != 2 || errorPage[0].size() != 3)
			throw std::invalid_argument("Invalid error page");

		if (!StringUtils::IsAllDigit(errorPage[0]))
			throw std::invalid_argument("Invalid error page");

		server.serverConfig.errorPage.push_back(
			CodePath(StrintToInt(errorPage[0]), errorPage[1])
		);
	}
}

void Parser::GetClientsBodySize(const Token& token, Server& server)
{
	if (token.tokenName == "client_max_body_size")
	{
		if (server.serverConfig.clientMaxBody.size.size() != 0)
			throw std::invalid_argument("Too many clientBodySize");

		server.serverConfig.clientMaxBody.InitClientBodySize(token.tokenValue);
	}
}

void Parser::GetLocationPath(const Token& token, Server& server)
{
	if (token.tokenName == "location")
	{
		if (token.tokenValue.size() == 0 || token.tokenValue[0] != '/')
			throw std::invalid_argument("invalid location");
			
		server.serverConfig.locations.push_back(Location(token.tokenValue));
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
				throw std::invalid_argument("invalid method");

			server.serverConfig.GetLastLocation().methods.push_back(methods[i]);
		}
	}
}

void Parser::GetRedirect(const Token& token, Server& server)
{
	if (token.tokenName == "return")
	{
		std::vector<std::string> redirect;

		if (server.serverConfig.GetLastLocation().redirect.code != 0)
				throw std::invalid_argument("too many redirect");

		redirect = StringUtils::Split(token.tokenValue, ' ');

		if (redirect.size() == 2)
		{
			if (!StringUtils::IsAllDigit(redirect[0]))
				throw std::invalid_argument("invalid redirect");

			server.serverConfig.GetLastLocation().redirect = CodePath(StrintToInt(redirect[0]),redirect[1]);
		}
		else if (redirect.size() == 1)
			server.serverConfig.GetLastLocation().redirect = CodePath(302, redirect[0]);
		else
			throw std::invalid_argument("invalid redirect");
	}
}

void Parser::GetRootPath(bool isServer,const Token& token, Server& server)
{
	if (token.tokenName == "root")
	{
		if (isServer)
		{
			if(server.serverConfig.serverRoot != "")
				throw std::invalid_argument("too many roots");

			server.serverConfig.serverRoot = token.tokenValue;
		}
		else
		{
			for (size_t i = 0; i < server.serverConfig.locations.size(); i++)
			{
				if (server.serverConfig.locations[i].rootPath.size() != 0)
					throw std::invalid_argument("too many roots");

				server.serverConfig.locations[i].rootPath = token.tokenValue;
			}
		}
	}
}

void Parser::GetAutoIndex(bool isServer, const Token& token, Server& server)
{
	if (token.tokenName == "auto_index")
	{
		if (isServer)
		{
			if (server.serverConfig.isInitAutoindex)
				throw std::invalid_argument("too many autoindex");

			server.serverConfig.isInitAutoindex = true;
			if (token.tokenValue == "on")
				server.serverConfig.autoIndex = true;
			else if (token.tokenValue == "off")
				server.serverConfig.autoIndex = false;
			else
				throw std::invalid_argument("too many autoindex");
		}
		else
		{
			if (server.serverConfig.GetLastLocation().isInitAutoindex)
				throw std::invalid_argument("too many autoindex");

			server.serverConfig.GetLastLocation().isInitAutoindex = true;
			if (token.tokenValue == "on")
				server.serverConfig.GetLastLocation().autoIndex = true;
			else if (token.tokenValue == "off")
				server.serverConfig.GetLastLocation().autoIndex = false;
			else
				throw std::invalid_argument("too many autoindex");
		}
	}
}

void Parser::GetIndex(bool isServer, const Token& token, Server& server)
{
	if (token.tokenName == "index")
	{
		std::vector<std::string> index;
		index = StringUtils::Split(token.tokenValue, ' ');
		for (size_t j = 0; j < index.size(); j++)
		{
			if (index[j].size() == 0)
				throw std::invalid_argument("invalid index");

			if (isServer)
				server.serverConfig.indexPages.push_back(index[j]);
			else
				server.serverConfig.GetLastLocation().index.push_back(index[j]);
		}
	}
}

void Parser::GetCgiExtension(const Token& token, Server& server)
{
	if (token.tokenName == "cgi_extension")
	{
		if(!server.serverConfig.GetLastLocation().cgiExtension.empty())
			throw std::invalid_argument("Too many cgiExtensions");

		if (token.tokenValue.size() == 0 || token.tokenValue[0] != '.')
			throw std::invalid_argument("Too many cgiExtensions");

		server.serverConfig.GetLastLocation().cgiExtension = token.tokenValue;
	}
}

void Parser::GetCgiPath(const Token& token, Server& server)
{
	if (token.tokenName == "cgi_path")
	{
		if(!server.serverConfig.GetLastLocation().cgiPath.empty())
			throw std::invalid_argument("Too many cgipath");

		server.serverConfig.GetLastLocation().cgiPath = token.tokenValue;
	}
}

void Parser::GetUploadPath(const Token& token, Server& server)
{
	if (token.tokenName == "upload_path")
	{
		if(!server.serverConfig.GetLastLocation().uploadPath.empty())
			throw std::invalid_argument("Too many cgipath");

		server.serverConfig.GetLastLocation().uploadPath = token.tokenValue;
	}
}

void Parser::GetUploadEnable(const Token& token, Server& server)
{
	if (token.tokenName == "upload_enable")
	{
		if(!server.serverConfig.GetLastLocation().uploadEnableFind == false)
			throw std::invalid_argument("Too many uploadEnables");

		server.serverConfig.GetLastLocation().uploadEnableFind = true;

		if (token.tokenValue == "on")
			server.serverConfig.GetLastLocation().uploadEnable = true;
		else if (token.tokenValue == "off")
			server.serverConfig.GetLastLocation().uploadEnable = false;
		else
			throw std::invalid_argument("invalid uploadEnables");
	}
}

