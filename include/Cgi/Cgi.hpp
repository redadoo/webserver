#ifndef CGI_HPP
# define CGI_HPP

#include <string>
#include <iostream>
#include <map>
#include <HttpMessage.hpp>
#include <HttpResponse.hpp>

class Cgi
{
	private:
		std::string interpreterPath;
		std::string scriptPath;
		std::map<std::string, std::string> env;

		void SetEnv(const HttpMessage& request, const std::string& serverName, int serverPort);
		std::string ExecuteCgi(const std::string& requestBody);
	public:
		Cgi(const std::string& interpreterPath, const std::string& scriptPath);
		HttpResponse ProcessCgiRequest(const HttpMessage& request, const std::string& serverName, int serverPort);

};

#endif
