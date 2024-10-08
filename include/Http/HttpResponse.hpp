#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

#include <HttpMessage.hpp>
#include <HttpStatusCode.hpp>
#include <ServerConfig.hpp>

# define UPLOAD_SUCCESS_BODY "<html><body><h1>Upload Successful</h1><p>Your file has been uploaded successfully.</p></body></html>"
# define NOT_FOUND_BODY_ERROR "<html><body><h1>404 Not Found</h1><p>The requested resource could not be found.</p></body></html>"
# define FORBIDDEN_BODY_ERROR "<html><body><h1>403 Forbidden</h1><p>You don't have permission to access this resource.</p></body></html>"
# define INTERNAL_SERVER_ERROR_BODY_ERROR "<html><body><h1>500 Internal Server Error</h1><p>The server encountered an unexpected condition that prevented it from fulfilling the request.</p></body></html>"
# define NOT_IMPLEMENTED_BODY_ERROR "<html><body><h1>501 Not Implemented</h1><p>The server does not support the functionality required to fulfill the request.</p></body></html>"
# define BAD_REQUEST_BODY_ERROR "<html><body><h1>400 Bad Request</h1><p>The server could not understand the request due to invalid syntax.</p></body></html>"
# define PAYLOAD_TOO_LARGE_BODY_ERROR "<html><body><h1>413 Payload Too Large</h1><p>The request is larger than the server is willing or able to process.</p></body></html>"
# define OK_ERROR "<html><body><h1>Request processed Successful</h1><p>Your request has been processed successfully.</p></body></html>"

class HttpResponse : public HttpMessage
{
	public:
    	HttpStatusCode::Code code;

		HttpResponse();

		void SetUploadBody();

		/// @brief Sets the response body for error pages based on the server configuration. If a custom error page is specified, it is used; otherwise, a default error page is provided.
		/// @param config The server configuration used to determine if a custom error page should be used.
		void SetErrorBody(const ServerConfig& config);

		/// @brief Sets the HTTP status code for the response and updates the response start line with the corresponding reason phrase.
		/// @param statusCode The HTTP status code to set for the response.
		void SetStatusCode(HttpStatusCode::Code statusCode);

		/// @brief Sets the body of the HTTP response and updates the content length header accordingly.
		/// @param body The content to be set as the body of the response.
		void SetBody(const Ustring& body);

		/// @brief Sets the body of the HTTP response and updates the content length header accordingly.
		/// @param body The content to be set as the body of the response.
		void SetBody(const std::string& body);

		/// @brief Sets the `Content-Type` header for the HTTP response.
		/// @param contentType The MIME type to be set in the `Content-Type` header.
		void SetContentType(const std::string& contentType);

		/// @brief Sets the `Content-Length` header for the HTTP response based on the length of the response body.
		/// This method updates the header to reflect the size of the response body in bytes.
		void SetContentLength();

		/// @brief Retrieves the path to a custom error page based on the HTTP status code, as specified in the server configuration.
		/// @param config The server configuration containing custom error page mappings.
		/// @return The path to the custom error page if it exists; otherwise, returns an empty string.
		std::string GetCustomErrorPage(const ServerConfig& config) const;

		/// @brief Converts the HTTP response into a string representation suitable for sending over a network.
		/// @return A string containing the full HTTP response, including the start line, headers, and body.
		std::string ToString() const;

	private:
		/// @brief Generates the HTML body for error responses based on the HTTP status code.
		/// @param code The HTTP status code for which the error body is to be generated.
		/// @return A string containing the HTML body for the error response.
		std::string GetErrorBody(HttpStatusCode::Code code);

};

#endif
