#ifndef PARSER_HPP
# define PARSER_HPP

# include <Server.hpp>
# include <Location.hpp>
# include <CodePath.hpp>
# include <ClientBodySize.hpp>

class Parser
{
	private:

		bool isDomain(std::string str);
		void resetValues();
		void getServerStartEnd();
		void getLocationStartEnd();

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
		static void ParseConfigFile(std::vector<Server> &servers, const char *fileConf);

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

#endif
