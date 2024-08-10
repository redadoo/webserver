#ifndef SIGNAL_HHP
#define SIGNAL_HHP

# include <WebServerException.hpp>
# include <csignal>

namespace WebServerSignal
{
	struct SignalState  { bool signCaught; };

	void SetupSignalHandler();
	
	void HandleSigstp(int sig);
	
} // namespace Signal




#endif