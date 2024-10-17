#include <WebServer.hpp>
#include <Logger.hpp>

int main(int argc, char **argv)
{
	Logger::Init();
	WebServer *webServer = new WebServer();

	const char *configFile = (argc == 2) ? argv[1] : "config/default.conf";

	try {
		webServer->InitServer(configFile);
		webServer->StartServer();
	}
	catch(const std::exception& e) {
		Logger::LogException(e);
	}
	
	delete webServer;
}