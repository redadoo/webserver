#ifndef WEBSERBEREXCEPTION_HPP
#define WEBSERBEREXCEPTION_HPP

#include <exception>

namespace WebServerExceptions
{
    class ErrorOnSocketCreation : public std::exception {
        public:
            virtual const char* what() const throw() { return "error when try to create socket";}
    };

    class ErrorOnBindSocket : public std::exception {
	    public:
		    virtual const char* what() const throw() { return "error when try to bind socket";}
    };

    class ErrorOnListenSocket : public std::exception {
	    public:
		    virtual const char* what() const throw() { return "error when try to listen socket";}
    };

    class ErrorOnEpollCreation : public std::exception {
	    public:
		    virtual const char* what() const throw() { return "error when try to create epoll instance";}
    };

    class ErrorOnAddSocketToEpoll : public std::exception {
	    public:
		    virtual const char* what() const throw() { return "error when try to add server socket to epoll instance";}
    };
    
    class ErrorOnFileConfigurationSyntax : public std::exception {
        public:
			virtual const char* what() const throw() { return "error when try to parse configuration file";}
	};

    class FileNotFound : public std::exception {
        public:
			virtual const char* what() const throw() { return "error: file not found";}
	};
    
} // namespace WebServerExceptions


#endif