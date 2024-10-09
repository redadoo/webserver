#pragma once
#ifndef CGI_HPP
# define CGI_HPP

#include <string>
#include <iostream>
#include <map>
#include <HttpMessage.hpp>
#include <HttpResponse.hpp>
#include <Server.hpp>

class Cgi
{
	private:
		std::string interpreterPath;
		std::string scriptPath;
		std::map<std::string, std::string> env;

		void SetEnv(HttpMessage& request, const std::string& serverName, int serverPort);
		std::string ExecuteCgi(Server& server, const Client& client);
	public:
		Cgi(const std::string& interpreterPath, const std::string& scriptPath);
		HttpResponse ProcessCgiRequest(Server& server, const Client& client, HttpMessage& request, const std::string& serverName, int serverPort);

};

#endif
