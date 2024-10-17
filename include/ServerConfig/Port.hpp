#pragma once

#ifndef PORT_HPP
# define PORT_HPP

#include <stdint.h>
#include <string>

struct Port
{
	uint16_t port;

	//default constructor
	Port() : port(0) {};

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

#endif // !PORT_HPP