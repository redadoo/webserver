#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

#include <HttpMessage.hpp>
#include <HttpStatusCode.hpp>
#include <ServerConfig.hpp>

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
