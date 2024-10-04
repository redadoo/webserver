#include <HandleSignal.hpp>
#include <Logger.hpp>
#include <cstring>

extern  HandleSignal::SignalState signalState;

void HandleSignal::HandleSigstp(int sig)
{
    if (sig == SIGINT || sig == SIGTSTP || sig == SIGQUIT)
        signalState.signCaught = true;
}

void HandleSignal::SetupSignalHandler()
{
	Logger::Log("signal handling started");

    struct sigaction	sa;

	signalState.signCaught = false;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler =  HandleSignal::HandleSigstp;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	// Set up the sigaction to handle SIGINT (Ctrl + C)
	if (sigaction(SIGINT, &sa, NULL) == -1)
		throw std::invalid_argument("Error: cannot handle SIGINT");

	// Set up the sigaction to handle SIGTSTP (Ctrl + Z)
	if (sigaction(SIGTSTP, &sa, NULL) == -1)
		throw std::invalid_argument("Error: cannot handle SIGTSTP");

	// Set up the sigaction to handle SIGQUIT (Ctrl + \)
    if (sigaction(SIGQUIT, &sa, NULL) == -1)
		throw std::invalid_argument("Error: cannot handle SIGQUIT");

	Logger::Log("handled the signals successfully");

}