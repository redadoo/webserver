#ifndef PARSER_HPP
# define PARSER_HPP

# include <Server.hpp>
# include <Location.hpp>
# include <CodePath.hpp>
# include <ClientBodySize.hpp>
# include <Lexer.hpp>

using namespace Lexer;

namespace Parser
{
	void ParseConfigFile(std::vector<Server> &servers, const char *fileConf);

	void GetPort(const Token& token, Server& server);
	void GetHost(const Token& token, Server& server);
	void GetServerName(const Token& token, Server& server);
	void GetErrorPage(const Token& token, Server& server);
	void GetClientsBodySize(const Token& token, Server& server);
	void GetLocationPath(const Token& token, Server& server);
	void GetMethods(const Token& token, Server& server);
	void GetRedirect(const Token& token, Server& server);
	void GetRootPath(bool isServer, const Token& token, Server& server);
	void GetAutoIndex(bool isServer, const Token& token, Server& server);
	void GetIndex(bool isServer, const Token& token, Server& server);
	void GetCgiExtension(const Token& token, Server& server);
	void GetCgiPath(const Token& token, Server& server);
	void GetUploadPath(const Token& token, Server& server);
	void GetUploadEnable(const Token& token, Server& server);

}

#endif
