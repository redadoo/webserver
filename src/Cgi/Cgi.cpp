#include <Cgi.hpp>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <stdexcept>
#include <cstring>
#include <sstream>
#include <Logger.hpp>
#include <StringUtils.hpp>
#include <FIleUtils.hpp>

Cgi::Cgi(const std::string& interpreterPath, const std::string& scriptPath)
	: interpreterPath(interpreterPath), scriptPath(scriptPath) {}

void Cgi::SetEnv(const HttpMessage& request, const std::string& serverName, int serverPort)
{

	env["SERVER_SOFTWARE"] = "WebServer/1.0";
	env["SERVER_NAME"] = serverName;
	env["GATEWAY_INTERFACE"] = "CGI/1.1";
	env["SERVER_PROTOCOL"] = "HTTP/1.1";
	env["SERVER_PORT"] = StringUtils::ToString(serverPort);
	env["REQUEST_METHOD"] = request.startLine.httpMethod;
	env["SCRIPT_NAME"] = scriptPath;

	std::pair<std::string, std::string> pathInfo = StringUtils::SplitPathAndQuery(request.startLine.path);

	env["PATH_INFO"] = pathInfo.first;
	if (!pathInfo.second.empty())
		env["QUERY_STRING"] = pathInfo.second;

	Header::const_iterator contentType = request.header.find("Content-Length:");
	if (contentType != request.header.end())
		env["CONTENT_LENGTH"] = contentType->second;

	Header::const_iterator contentLength = request.header.find("Content-Type:");
	if (contentLength != request.header.end())
		env["CONTENT_TYPE"] = contentLength->second;

	for (Header::const_iterator it = request.header.begin(); it != request.header.end(); ++it)
	{
		std::string headerName = it->first;
		headerName.erase(headerName.length() - 1);
		std::string envName = "HTTP_" + headerName;
		for (std::string::iterator ch = envName.begin(); ch != envName.end(); ++ch)
		{
			if (*ch == '-')
				*ch = '_';
			else
				*ch = std::toupper(*ch);
		}
		env[envName] = it->second;
	}
}

std::string Cgi::ExecuteCgi(const std::string& requestBody)
{
	int pipefd[2];
	if (pipe(pipefd) == -1)
		throw std::runtime_error("Error creating pipe for CGI execution");

	pid_t pid = fork();
	if (pid == -1)
		throw std::runtime_error("Error forking process for CGI execution");

	if (pid == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);

		for (std::map<std::string, std::string>::const_iterator it = env.begin(); it != env.end(); ++it)
			setenv(it->first.c_str(), it->second.c_str(), 1);

		execl(interpreterPath.c_str(), interpreterPath.c_str(), scriptPath.c_str(), NULL);
		exit(EXIT_FAILURE);
	}
	else
	{
		close(pipefd[1]);

		if (!requestBody.empty())
		{
			write(STDIN_FILENO, requestBody.c_str(), requestBody.length());
			close(STDIN_FILENO);
		}

		std::string output;
		char buffer[4096];
		ssize_t bytesRead;
		while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer))) > 0)
			output.append(buffer, bytesRead);

		close(pipefd[0]);

		int status;
		waitpid(pid, &status, 0);

		if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
			return output;
		else
			throw std::runtime_error("Error executing CGI script");
	}
}

HttpResponse Cgi::ProcessCgiRequest(const HttpMessage& request, const std::string& serverName, int serverPort)
{
	SetEnv(request, serverName, serverPort);

	try
	{
		std::string cgiOutput = ExecuteCgi(request.body);

		HttpResponse response;
		std::istringstream cgiStream(cgiOutput);
		std::string line;
		bool headersDone = false;

		while (std::getline(cgiStream, line) && !headersDone)
		{
			if (line == "\r" || line.empty())
				headersDone = true;
			else
			{
				std::string::size_type pos = line.find(':');
				if (pos != std::string::npos)
				{
					std::string headerName = line.substr(0, pos);
					std::string headerValue = line.substr(pos + 1);
					while (!headerValue.empty() && (headerValue[0] == ' ' || headerValue[0] == '\t'))
						headerValue.erase(0, 1);
					response.header[headerName] = headerValue;
				}
			}
		}

		std::stringstream bodyStream;
		bodyStream << cgiStream.rdbuf();
		response.body = bodyStream.str();

		Header::const_iterator statusIt = response.header.find("Status:");
		if (statusIt != response.header.end())
		{
			int statusCode = StringUtils::StrintToInt(statusIt->second.substr(0, 3));
			response.SetStatusCode(static_cast<HttpStatusCode::Code>(statusCode));
			response.header.erase("Status:");
		}
		else
			response.SetStatusCode(HttpStatusCode::OK);

		return response;
	}
	catch (const std::exception& ex)
	{
		Logger::LogError("Error processing CGI request: " + std::string(ex.what()));
		HttpResponse response;
		response.SetStatusCode(HttpStatusCode::InternalServerError);
		response.SetBody("CGI execution error");
		return response;
	}
}
