#include <HttpResponse.hpp>
#include <HttpStatusCode.hpp>
#include <StringUtils.hpp>
#include <FIleUtils.hpp>
#include <Logger.hpp>

std::string HttpResponse::GetErrorBody(HttpStatusCode::Code code)
{
	switch (code)
	{
		 case HttpStatusCode::NotFound:
            return "<html><body><h1>404 Not Found</h1><p>The requested resource could not be found.</p></body></html>";
        case HttpStatusCode::Forbidden:
            return "<html><body><h1>403 Forbidden</h1><p>You don't have permission to access this resource.</p></body></html>";
        case HttpStatusCode::InternalServerError:
            return "<html><body><h1>500 Internal Server Error</h1><p>The server encountered an unexpected condition that prevented it from fulfilling the request.</p></body></html>";
		case HttpStatusCode::NotImplemented:
			return "<html><body><h1>501 Not Implemented</h1><p>The server does not support the functionality required to fulfill the request.</p></body></html>";
		case HttpStatusCode::BadRequest:
			return "<html><body><h1>400 Bad Request</h1><p>The server could not understand the request due to invalid syntax.</p></body></html>";
		case HttpStatusCode::PayloadTooLarge:
			return "<html><body><h1>413 Payload Too Large</h1><p>The request is larger than the server is willing or able to process.</p></body></html>";

        default:
            return "<html><body><h1>Error " + HttpStatusCode::reasonPhrase(code) + "</h1><p>An error occurred while processing your request.</p></body></html>";
	}
}

std::string HttpResponse::GetCustomErrorPage(const ServerConfig& config) const
{
	for (std::vector<CodePath>::const_iterator it = config.errorPage.begin(); it != config.errorPage.end(); ++it)
	{
		if (it->code == code)
		{
			if (FileUtils::CheckFileExistence(it->path.c_str()))
			{
				return it->path;
			}
			else
			{
				Logger::Log("Custom error page not found: " + it->path);
				break;
			}
		}
	}
	return "";
}

HttpResponse::HttpResponse() : code(HttpStatusCode::OK)
{
	startLine.httpVersion = "HTTP/1.1";
}

void HttpResponse::SetStatusCode(HttpStatusCode::Code statusCode)
{
	code = statusCode;
	startLine.httpMethod = HttpStatusCode::reasonPhrase(statusCode);
}

void HttpResponse::SetBody(const std::string& body)
{
	this->body = body;
	SetContentLength();
}

void HttpResponse::SetErrorBody(const ServerConfig& config)
{
	std::string customErrorPage = GetCustomErrorPage(config);
	if (!customErrorPage.empty())
	{
		SetBody(FileUtils::ReadFile(customErrorPage));
		SetContentType(FileUtils::GetContentType(customErrorPage));
		Logger::Log("Using custom error page: " + customErrorPage);
	}
	else
	{
		SetBody(GetErrorBody(code));
		SetContentType("text/html");
		Logger::Log("Using default error page for code " + HttpStatusCode::reasonPhrase(code));
	}

}


void HttpResponse::SetContentType(const std::string& contentType)
{
	header["Content-Type"] = contentType;
}

void HttpResponse::SetContentLength()
{
	header["Content-Length"] = StringUtils::ToString(static_cast<int>(body.length()));
}

std::string HttpResponse::ToString() const
{
	std::stringstream ss;


	ss << startLine.httpVersion << " " << HttpStatusCode::reasonPhrase(code) << "\r\n";

	for (Header::const_iterator it = header.begin(); it != header.end(); ++it)
	{
		ss << it->first << ": " << it->second << "\r\n";
	}

	ss << "\r\n" << body;

	ss << body;

	return ss.str();
}
