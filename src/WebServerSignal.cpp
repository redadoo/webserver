#include <WebServerSignal.hpp>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <string.h>

extern  WebServerSignal::SignalState signalState;

void WebServerSignal::HandleSigstp(int sig)
{
	switch (sig) {
        case SIGINT:
            signalState.signCaught = true;
            break;
        case SIGTSTP:
            signalState.signCaught = true;
            break;
        case SIGQUIT:
            signalState.signCaught = true;
            break;
        default:
            break;
    }
}

void WebServerSignal::SetupSignalHandler()
{
    struct sigaction	sa;

	signalState.signCaught = false;
	
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler =  WebServerSignal::HandleSigstp;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	// Set up the sigaction to handle SIGINT (Ctrl + C)
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		std::cerr << "Error: cannot handle SIGINT" << std::endl;
		std::exit(1);
	}

	// Set up the sigaction to handle SIGTSTP (Ctrl + Z)
	if (sigaction(SIGTSTP, &sa, NULL) == -1)
	{
		std::cerr << "Error: cannot handle SIGTSTP" << std::endl;
		std::exit(1);
	}

	// Set up the sigaction to handle SIGQUIT (Ctrl + \)
    if (sigaction(SIGQUIT, &sa, NULL) == -1) {
        std::cerr << "Error: cannot handle SIGQUIT" << std::endl;
        std::exit(EXIT_FAILURE);  // Use std::exit from <cstdlib>
    }
}