#ifndef PARSER_HPP
# define PARSER_HPP

# include <Server.hpp>
# include <Location.hpp>
# include <CodePath.hpp>
# include <ClientBodySize.hpp>
# include <Lexer.hpp>

using namespace Lexer;

namespace Parser
{
	void ParseConfigFile(std::vector<Server> &servers, const char *fileConf);

	void GetServerStartEnd();
	void GetLocationStartEnd();

	void GetPort(const Token& token, Server& server);
	void GetHost(const Token& token, Server& server);
	void GetServerName(const Token& token, Server& server);
	void GetErrorPage(const Token& token, Server& server);
	void CheckErrorPage(const std::vector<std::string> &errorPage, Server& server);
	void GetClientsBodySize(const Token& token, Server& server);
	void GetLocationPath(const Token& token, Server& server);
	void GetMethods(const Token& token, Server& server);
	void CheckMethod(const std::string &method);
	void GetRedirect(const Token& token, Server& server);
	void CheckRedirect(const std::vector<std::string> &redirect);
	void GetRootPath(bool isServer, const Token& token, Server& server);
	void GetAutoIndex(bool isServer, const Token& token, Server& server);
	void GetIndex(bool isServer, const Token& token, Server& server);
	void GetCgiExtension(const Token& token, Server& server);
	void GetCgiPath(const Token& token, Server& server);
	void GetUploadPath(const Token& token, Server& server);
	void GetUploadEnable(const Token& token, Server& server);
	void SetupServerConfig(const Token& token, Server& server);

class EmptyFile : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Empty file";
			}
	};

	class FirstTokenNotServer : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "First token is not server";
			}
	};

	class TooManyPorts : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Too many ports";
			}
	};

	class InvalidPort : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Invalid port";
			}
	};

	class PortNotFound : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Port not found";
			}
	};

	class TooManyHosts : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Too many hosts";
			}
	};

	class InvalidHost : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Invalid host";
			}
	};

	class HostNotFound : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Host not found";
			}
	};

	class InvalidServerName : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Invalid server name";
			}
	};

	class InvalidErrorPage : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Invalid error page";
			}
	};

	class ErrorPageNotFound : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Error page not found";
			}
	};

	class DuplicateErrorPage : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Duplicate error page";
			}
	};

	class InvalidClientBodySize : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Invalid client body size";
			}
	};

	class TooManyClientBodySize : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Too many client body size";
			}
	};

	class ClientBodySizeNotFound : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Client body size not found";
			}
	};

	class InvalidLocation : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Invalid location";
			}
	};

	class InvalidMethod : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Invalid method";
			}
	};

	class InvalidRedirect : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Invalid redirect";
			}
	};

	class TooManyRedirects : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Too many redirects";
			}
	};

	class RootNotFound : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Root not found";
			}
	};

	class TooManyRoots : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Too many roots";
			}
	};

	class TooManyAutoIndexes : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Too many auto index";
			}
	};

	class InvalidAutoIndex : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Invalid auto index";
			}
	};

	class InvalidIndex : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Invalid index";
			}
	};

	class TooManyCgiExtensions : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Too many cgi extensions";
			}
	};

	class InvalidCgiExtension : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Invalid cgi extension";
			}
	};

	class TooManyCgiPaths : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Too many cgi paths";
			}
	};

	class TooManyUploadPaths : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Too many upload paths";
			}
	};

	class TooManyUploadEnables : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Too many upload enables";
			}
	};

	class InvalidUploadEnable : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Invalid upload enable";
			}
	};

}
	
#endif
