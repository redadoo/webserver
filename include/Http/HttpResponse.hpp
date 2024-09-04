#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

#include <HttpMessage.hpp>
#include <HttpStatusCode.hpp>
#include <ServerConfig.hpp>

# define NOT_FOUND_BODY_ERROR "<html><body><h1>404 Not Found</h1><p>The requested resource could not be found.</p></body></html>"
# define FORBIDDEN_BODY_ERROR "<html><body><h1>403 Forbidden</h1><p>You don't have permission to access this resource.</p></body></html>"
# define INTERNAL_SERVER_ERROR_BODY_ERROR "<html><body><h1>500 Internal Server Error</h1><p>The server encountered an unexpected condition that prevented it from fulfilling the request.</p></body></html>"
# define NOT_IMPLEMENTED_BODY_ERROR "<html><body><h1>501 Not Implemented</h1><p>The server does not support the functionality required to fulfill the request.</p></body></html>"
# define BAD_REQUEST_BODY_ERROR "<html><body><h1>400 Bad Request</h1><p>The server could not understand the request due to invalid syntax.</p></body></html>"
# define PAYLOAD_TOO_LARGE_BODY_ERROR "<html><body><h1>413 Payload Too Large</h1><p>The request is larger than the server is willing or able to process.</p></body></html>"

class HttpResponse : public HttpMessage
{
	private:
		std::string GetErrorBody(HttpStatusCode::Code code);

	public:
    	HttpStatusCode::Code code;

		HttpResponse();

		void SetErrorBody(const ServerConfig& config);
		void SetStatusCode(HttpStatusCode::Code statusCode);
		void SetBody(const std::string& body);
		void SetContentType(const std::string& contentType);
		void SetContentLength();
		std::string GetCustomErrorPage(const ServerConfig& config) const;
		std::string ToString() const;

};

#endif
