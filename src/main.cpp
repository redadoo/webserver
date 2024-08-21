# include <WebServer.hpp>
# include <Logger.hpp>

int main(int argc, char **argv)
{
	WebServer *webServer;
	const char *configFile = (argc == 2) ? argv[1] : "config/default.conf";
	
	Logger::Init();

	webServer = new WebServer();
	try {
		webServer->InitServer(configFile);
		// webServer->StartServer();
	}
	catch(const std::exception& e) {
		Logger::LogException(e);
	}

	Logger::Log("Closing WebServer ...");
	
	delete webServer;
	
	Logger::Log("all resources released and Web Server closed successfully");
	
}