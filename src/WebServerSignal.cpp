# include <WebServerSignal.hpp>
# include <Logger.hpp>
# include <cstring>

extern  WebServerSignal::SignalState signalState;

void WebServerSignal::HandleSigstp(int sig)
{
    if (sig == SIGINT || sig == SIGTSTP || sig == SIGTSTP)
        signalState.signCaught = true;
}

void WebServerSignal::SetupSignalHandler()
{
	Logger::Log("signal handling started");

    struct sigaction	sa;

	signalState.signCaught = false;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler =  WebServerSignal::HandleSigstp;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	// Set up the sigaction to handle SIGINT (Ctrl + C)
	if (sigaction(SIGINT, &sa, NULL) == -1)
		throw ErrorOnCannotHandleSigint();

	// Set up the sigaction to handle SIGTSTP (Ctrl + Z)
	if (sigaction(SIGTSTP, &sa, NULL) == -1)
		throw ErrorOnCannotHandleSigquit();

	// Set up the sigaction to handle SIGQUIT (Ctrl + \)
    if (sigaction(SIGQUIT, &sa, NULL) == -1)
		throw ErrorOnCannotHandleSigtstp();

	Logger::Log("handled the signals successfully");

}