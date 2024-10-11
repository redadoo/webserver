#pragma once
#ifndef PARSER_HPP
# define PARSER_HPP

#include <Server.hpp>
#include <Lexer.hpp>

using namespace Lexer;

namespace Parser
{
	/// @brief Parses the configuration file and populates the servers vector with Server objects.
	/// @param servers A vector to store the parsed Server objects.
	/// @param fileConf The path to the configuration file to be parsed.
	void ParseConfigFile(std::vector<Server> &servers, const char *fileConf);

	void FillServer(const Token& token, Server& server);

	void FillLocation(const Token& token, Server& server);

	/// @brief Extracts and sets the port configuration from the token.
	/// @param token The token containing port information.
	/// @param server The server to update with the port information.
	void GetPort(const Token& token, Server& server);

	/// @brief Extracts and sets the host configuration from the token.
	/// @param token The token containing host information.
	/// @param server The server to update with the host information.
	void GetHost(const Token& token, Server& server);

	/// @brief Extracts and sets the server name configuration from the token.
	/// @param token The token containing server name information.
	/// @param server The server to update with the server name information.
	void GetServerName(const Token& token, Server& server);

	/// @brief Extracts and sets the error page configuration from the token.
	/// @param token The token containing error page information.
	/// @param server The server to update with the error page information.
	void GetErrorPage(const Token& token, Server& server);

	/// @brief Extracts and sets the client body size configuration from the token.
	/// @param token The token containing client body size information.
	/// @param server The server to update with the client body size information.
	void GetClientsBodySize(const Token& token, Server& server);

	/// @brief Extracts and sets the location path configuration from the token.
	/// @param token The token containing location path information.
	/// @param server The server to update with the location path information.
	void GetLocationPath(const Token& token, Server& server);

	/// @brief Extracts and sets the allowed methods configuration from the token.
	/// @param token The token containing allowed methods information.
	/// @param server The server to update with the allowed methods information.
	void GetMethods(const Token& token, Server& server);

	/// @brief Extracts and sets the redirect configuration from the token.
	/// @param token The token containing redirect information.
	/// @param server The server to update with the redirect information.
	void GetRedirect(const Token& token, Server& server);

	/// @brief Extracts and sets the root path configuration from the token.
	/// @param isServer Indicates whether the root path is for the server or location.
	/// @param token The token containing root path information.
	/// @param server The server to update with the root path information.
	void GetRootPath(bool isServer, const Token& token, Server& server);

	/// @brief Extracts and sets the auto index configuration from the token.
	/// @param isServer Indicates whether the auto index is for the server or location.
	/// @param token The token containing auto index information.
	/// @param server The server to update with the auto index information.
	void GetAutoIndex(bool isServer, const Token& token, Server& server);

	/// @brief Extracts and sets the index pages configuration from the token.
	/// @param isServer Indicates whether the index pages are for the server or location.
	/// @param token The token containing index pages information.
	/// @param server The server to update with the index pages information.
	void GetIndex(bool isServer, const Token& token, Server& server);

	/// @brief Extracts and sets the CGI extension configuration from the token.
	/// @param token The token containing CGI extension information.
	/// @param server The server to update with the CGI extension information.
	void GetCgiExtension(const Token& token, Server& server);

	/// @brief Extracts and sets the CGI path configuration from the token.
	/// @param token The token containing CGI path information.
	/// @param server The server to update with the CGI path information.
	void GetCgiPath(const Token& token, Server& server);

	/// @brief Extracts and sets the upload path configuration from the token.
	/// @param token The token containing upload path information.
	/// @param server The server to update with the upload path information.
	void GetUploadPath(const Token& token, Server& server);

	/// @brief Extracts and sets the upload enable configuration from the token.
	/// @param token The token containing upload enable information.
	/// @param server The server to update with the upload enable information.
	void GetUploadEnable(const Token& token, Server& server);
}

#endif // PARSER_HPP
