#pragma once
#ifndef PORT_HPP
# define PORT_HPP

#include <stdint.h>
#include <string>

class Port
{
public:
	uint16_t port;

	Port() : port(0) {};

 	/// @brief Initializes the port with an integer value.
    /// @param _port The port number to set.
    /// @throws std::invalid_argument If the port number is out of the valid range (1-65535).
	void InitPort(int _port);

	/// @brief Initializes the port with a string value.
    /// @param _port The port number as a string to set.
    /// @throws std::invalid_argument If the port number is invalid or out of the valid range (1-65535).
	void InitPort(std::string _port);

    /// @brief Compares two Port objects for equality.
    /// @param p The Port object to compare with.
    /// @return True if the ports are equal, false otherwise.
	bool operator == (const Port &p);

    /// @brief Overloads the << operator to output server configuration details.
	/// @param os Output stream object.
	/// @param pr Reference to the Port object.
	/// @return Reference to the output stream object.
	friend std::ostream& operator<<(std::ostream& os, const Port& pr);

};

#endif