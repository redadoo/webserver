#pragma once
#ifndef WEBSERBEREXCEPTION_HPP
# define WEBSERBEREXCEPTION_HPP

#include <cstring>
#include <stdexcept>
#include <HttpStatusCode.hpp>

namespace WebServerException
{
	class ExceptionErrno : public std::invalid_argument 
	{
		public:
			ExceptionErrno(const std::string& msg, int err)
				: std::invalid_argument(msg + " : " + std::strerror(err)) {}

			virtual ~ExceptionErrno() throw() {}
	};

	class HttpStatusCodeException : public std::exception {
       public:
        HttpStatusCode::Code code;
        HttpStatusCodeException(HttpStatusCode::Code _code) : code(_code){};
        HttpStatusCodeException(int _code) : code(static_cast<HttpStatusCode::Code>(_code)){};

        ~HttpStatusCodeException() throw() {}

        virtual const char* what() const throw() {
            return HttpStatusCode::ReasonPhrase(code).c_str();
        };
    };
	
} // namespace WebServerExceptions

#endif