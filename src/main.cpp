# include <WebServer.hpp>
# include <Logger.hpp>

int main(int argc, char **argv)
{
	WebServer *webServer;

	Logger::Init();
	
	if (argc == 1)
		webServer = new WebServer();
	if (argc == 2) 
		webServer = new WebServer(argv[1]);

	try {
		webServer->InitServer();
		webServer->StartServer();
	}
	catch(const std::exception& e) {
		Logger::LogException(e);
	}

	Logger::Log("Closing WebServer ...");
	
	delete webServer;
	
	Logger::Log("WebServer closed ");
	
}