#ifndef WEBSERBEREXCEPTION_HPP
# define WEBSERBEREXCEPTION_HPP

# include <exception>
# include <string>
# include <cerrno>  
# include <cstring>
# include <stdexcept>
namespace WebServerException
{
	class ExceptionErrno : public std::invalid_argument 
	{
	public:
		ExceptionErrno(const std::string& msg, int err)
			: std::invalid_argument(msg + " : " + std::strerror(err)) {}

		virtual ~ExceptionErrno() throw() {}
	};
	
} // namespace WebServerExceptions

#endif