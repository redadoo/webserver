#ifndef WEBSERBEREXCEPTION_HPP
# define WEBSERBEREXCEPTION_HPP

# include <exception>
# include <string>
# include <cerrno>  
# include <cstring>

namespace WebServerException
{
	class ExceptionsWithArguments : public std::exception {
		protected:
			std::string errorMessage;

		public:
			explicit ExceptionsWithArguments(const std::string& msg) : errorMessage(msg) {}

			virtual const char* what() const throw() {
				return errorMessage.c_str();
			}

			virtual ~ExceptionsWithArguments() throw() {}
	};

	class ExceptionErrno : public ExceptionsWithArguments {
		public:
			ExceptionErrno(const std::string& msg, int err)
				: ExceptionsWithArguments(msg + ": " + std::strerror(err)) {}

			virtual ~ExceptionErrno() throw() {}
	};

	class ErrorOnFileConfigurationSyntax : public std::exception {
		public:
			virtual const char* what() const throw() { return "Error: when try to parse configuration file";}
	};

	class FileNotFound : public std::exception {
		public:
			virtual const char* what() const throw() { return "Error: file not found";}
	};

	class ErrorOnCannotHandleSigint : public std::exception {
		public:
			virtual const char* what() const throw() { return "Error: cannot handle SIGINT";}
	};

	class ErrorOnCannotHandleSigquit : public std::exception {
		public:
			virtual const char* what() const throw() { return "Error: cannot handle SIGTSTP";}
	};

	class ErrorOnCannotHandleSigtstp : public std::exception {
		public:
			virtual const char* what() const throw() { return "Error: cannot handle SIGQUIT";}
	};


	
} // namespace WebServerExceptions

#endif