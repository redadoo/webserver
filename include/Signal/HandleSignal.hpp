#pragma once
#ifndef SIGNAL_HHP
# define SIGNAL_HHP

#include <csignal>
#include <exception>

namespace HandleSignal
{
	/// @brief Struct to hold the state of the signal handling.
    /// This structure contains a boolean flag that indicates whether a signal has been caught.
	struct SignalState  { bool signCaught; };

    /// @brief Sets up signal handlers for the web server.
    /// This function configures the signal handling for SIGINT, 
    /// SIGTSTP, and SIGQUIT using `sigaction`.
	void SetupSignalHandler();
	
	/// @brief Handles the signals SIGINT, SIGTSTP, and SIGQUIT.
    /// This function is called when the specified signals are caught, and it sets the `signCaught` flag to true.
    /// @param sig The signal number.
	void HandleSigstp(int sig);
	
} // namespace Signal




#endif