#pragma once
#ifndef CLIENTBODYSIZE_HPP
# define CLIENTBODYSIZE_HPP

#include <string>

enum Unit
{
	BYTE,
	KILOBYTE,
	MEGABYTE,
	GIGABYTE
};

/// @brief Structure representing the client body size with size and unit.
struct ClientBodySize
{
	unsigned long long	size;
	Unit				unit;

	ClientBodySize() : size(0), unit(BYTE) {}

    /// @brief Initializes the client body size from a string representation.
    /// @param str String representation of the client body size (e.g., "1024K").
    /// @throws std::invalid_argument If the string is not a valid size or unit.
	void InitClientBodySize(const std::string& str);


	unsigned long long ConvertToBytes() const;

	/// @brief Overloads the << operator to output the client body size.
    /// @param os Output stream object.
    /// @param cl Reference to the ClientBodySize object.
    /// @return Reference to the output stream object.
	friend std::ostream& operator<<(std::ostream& os, const ClientBodySize& cl);

};

#endif
