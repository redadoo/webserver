#include <WebServerSignal.hpp>

extern  WebServerSignal::SignalState signalState;

void WebServerSignal::HandleSigstp(int sig)
{
    if (sig == SIGINT || sig == SIGTSTP || sig == SIGTSTP)
	{
        signalState.signCaught = true;
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
		throw WebServerException::ErrorOnCannotHandleSigint();

	// Set up the sigaction to handle SIGTSTP (Ctrl + Z)
	if (sigaction(SIGTSTP, &sa, NULL) == -1)
		throw WebServerException::ErrorOnCannotHandleSigquit();

	// Set up the sigaction to handle SIGQUIT (Ctrl + \)
    if (sigaction(SIGQUIT, &sa, NULL) == -1)
		throw WebServerException::ErrorOnCannotHandleSigtstp();
}