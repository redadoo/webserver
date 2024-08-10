#include <WebServer.hpp>

int main(int argc, char **argv)
{
	WebServer *webServer;

	Logger::Init();
	
	if (argc == 1)
		webServer = new WebServer();
	if (argc == 2) 
		webServer = new WebServer(argv[1]);

	try
	{
		webServer->InitServer();
		webServer->StartServer();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	

	Logger::Log("End WebServer");
	
	delete webServer;
}