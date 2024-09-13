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
#include <WebServerException.hpp>

Cgi::Cgi(const std::string& interpreterPath, const std::string& scriptPath)
	: interpreterPath(interpreterPath), scriptPath(scriptPath) {}

void Cgi::SetEnv(HttpMessage& request, const std::string& serverName, int serverPort)
{
	env["SERVER_NAME"] = serverName;
	env["SERVER_PORT"] = StringUtils::ToString(serverPort);
	env["REQUEST_METHOD"] = request.startLine.httpMethod;

	std::pair<std::string, std::string> pathInfo = StringUtils::SplitPathAndQuery(request.startLine.path);

	env["PATH_INFO"] = pathInfo.first;
	if (!pathInfo.second.empty())
		env["QUERY_STRING"] = pathInfo.second;

	env["CONTENT_LENGTH"] = StringUtils::ToString(request.body.length());

	std::string boundary = StringUtils::GetBoundary(request.header["Content-Type:"]);
	std::vector<std::string> parts = StringUtils::SplitMultipartData(request.body, boundary);
	env["CONTENT_TYPE"] = request.header["Content-Type:"];

	env["CONTENT_SIZE"] = StringUtils::ToString(parts.size());
	for (std::vector<std::string>::size_type i = 0; i < parts.size(); ++i)
			env["CONTENT_" + StringUtils::ToString(i + 1)] = parts[i];
}

std::string Cgi::ExecuteCgi()
{
	if (!FileUtils::CheckFileExistence(interpreterPath.c_str()))
		throw WebServerException::HttpStatusCodeException(HttpStatusCode::InternalServerError);

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

		char* matrix[] = { (char*)interpreterPath.c_str(), (char*)scriptPath.c_str(), NULL };
		char **new_env = StringUtils::GetMatrix(env);
		execve(interpreterPath.c_str(), matrix, new_env);
		exit(EXIT_FAILURE);
	}
	else
	{
		close(pipefd[1]);

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

HttpResponse Cgi::ProcessCgiRequest(HttpMessage& request, const std::string& serverName, int serverPort)
{
	SetEnv(request, serverName, serverPort);

	try
	{
		std::istringstream cgiStream(ExecuteCgi());
		HttpResponse response;
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
