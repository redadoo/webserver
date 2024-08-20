#ifndef PARSER_HPP
# define PARSER_HPP

# include <Server.hpp>
# include <Lexer.hpp>

using namespace Lexer;

		enum Unit
		{
			BYTE,
			KILOBYTE,
			MEGABYTE,
			GIGABYTE
		};

		struct CodePath
		{
			int code;
			std::string path;

			CodePath() : code(0), path("") {};
			CodePath(int _code, std::string _path) : code(_code), path(_path) {};
		};

		struct ClientBodySize
		{
			std::string size;
			Unit unit;
		};


		struct Location
		{
			std::string path;
			std::vector<std::string> methods;
			CodePath redirect;
			std::string rootPath;
			bool autoIndex;
			std::vector<std::string> index;
			std::string cgiExtension;
			std::string cgiPath;
			std::string uploadPath;
			bool uploadEnable;
		};

class ConfFileParser
{
	private:
		std::vector<Token> tokens;
		std::vector<Server> &servers;
		int serverStart;
		int serverEnd;
		int locationStart;
		int locationEnd;

		int port;
		std::string host;
		std::vector<std::string> serverNames;
		std::vector<CodePath> errorPages;
		ClientBodySize clientBodySize;
		std::string rootPath;
		bool autoIndex;
		std::vector<std::string> index;

		std::vector<Location> locations;

		int stringToInt(std::string str);
		std::vector<std::string> split(std::string str, char delim);

		bool isDomain(std::string str);
		bool isIp(std::string str);
		void resetValues();
		void getServerStartEnd();
		void getLocationStartEnd();
		void setupServer();

		void getPort();
		void checkPort(const std::string &port);
		void getHost();
		void checkHost(const std::string &host);
		void getServerName();
		void checkServerName(const std::string &name);
		void getErrorPage();
		void checkErrorPage(const std::vector<std::string> &errorPage);
		void getClientsBodySize();
		void getLocationPath();
		void getMethods();
		void checkMethod(const std::string &method);
		void getRedirect();
		void checkRedirect(const std::vector<std::string> &redirect);
		void getRootPath(const std::string &mode);
		void getAutoIndex(const std::string &mode);
		void getIndex(const std::string &mode);
		void getCgiExtension();
		void getCgiPath();
		void getUploadPath();
		void getUploadEnable();
		void setupServerConfig();


	public:
		ConfFileParser(std::vector<Server> &Servers, const char *fileConf);

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
};

namespace Parser
{
    void FillServer(std::vector<Server>& Servers, const char *fileConf);
}

#endif
