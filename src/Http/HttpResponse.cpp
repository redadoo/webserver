#include <StringUtils.hpp>
#include <FIleUtils.hpp>
#include <Logger.hpp>

std::string HttpResponse::GetErrorBody(HttpStatusCode::Code code)
{
	switch (code)
	{
		case HttpStatusCode::NotFound:
			return NOT_FOUND_BODY_ERROR;
		case HttpStatusCode::Forbidden:
			return FORBIDDEN_BODY_ERROR;
		case HttpStatusCode::InternalServerError:
			return INTERNAL_SERVER_ERROR_BODY_ERROR;
		case HttpStatusCode::NotImplemented:
			return NOT_IMPLEMENTED_BODY_ERROR;
		case HttpStatusCode::BadRequest:
			return BAD_REQUEST_BODY_ERROR;
		case HttpStatusCode::PayloadTooLarge:
			return PAYLOAD_TOO_LARGE_BODY_ERROR;
		default:
			return "<html><body><h1>Error " + HttpStatusCode::ReasonPhrase(code) + "</h1><p>An error occurred while processing your request.</p></body></html>";
	}
}

std::string HttpResponse::GetCustomErrorPage(const ServerConfig& config) const
{
	for (std::vector<CodePath>::const_iterator it = config.errorPage.begin(); it != config.errorPage.end(); ++it)
	{
		if (it->code == code)
		{
			if (FileUtils::CheckFileExistence(it->path.c_str()))
				return it->path;
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
	isHeaderComplete = false;
	body = "";
}

void HttpResponse::SetUploadBody()
{
	SetBody(UPLOAD_SUCCESS_BODY);
	SetContentType("text/html");
}

void HttpResponse::SetStatusCode(HttpStatusCode::Code statusCode)
{
	code = statusCode;
	startLine.httpMethod = HttpStatusCode::ReasonPhrase(statusCode);
}

void HttpResponse::SetBody(const Ustring& body)
{
	this->body = body;
	SetContentLength();
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
		// TODO CHECK ReadFile RETURN
		SetBody(FileUtils::ReadFile(customErrorPage));
		// TODO CHECK GetContentType
		SetContentType(FileUtils::GetContentType(customErrorPage));
		Logger::Log("Using custom error page: " + customErrorPage);
	}
	else
	{
		SetBody(GetErrorBody(code));
		SetContentType("text/html");
		Logger::Log("Using default error page for code " + HttpStatusCode::ReasonPhrase(code));
	}

}


void HttpResponse::SetContentType(const std::string& contentType)
{
	header["Content-Type: "] = contentType;
}

void HttpResponse::SetContentLength()
{
	header["Content-Length: "] = StringUtils::ToString(static_cast<int>(body.size()));
}

std::string HttpResponse::ToString() const
{
	std::stringstream ss;

	ss << startLine.httpVersion << " " << HttpStatusCode::ReasonPhrase(code) << "\r\n";

	for (Header::const_iterator it = header.begin(); it != header.end(); ++it)
		ss << it->first << ": " << it->second << "\r\n";

	ss << "\r\n" << body;
	return ss.str();
}
