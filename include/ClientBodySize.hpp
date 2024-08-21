
#ifndef CLIENTBODYSIZE_HPP
# define CLIENTBODYSIZE_HPP

#include <iostream>

enum Unit
{
	BYTE,
	KILOBYTE,
	MEGABYTE,
	GIGABYTE
};


struct ClientBodySize
{
	std::string size;
	Unit unit;

	friend std::ostream& operator<<(std::ostream& os, const ClientBodySize& cl)
	{
		os << cl.size;

		if (cl.unit == BYTE)
			os << " BYTE";
		if (cl.unit == KILOBYTE)
			os << " KILOBYTE";
		if (cl.unit == MEGABYTE)
			os << " MEGABYTE";
		if (cl.unit == GIGABYTE)
			os << " GIGABYTE";

		return os;
	}

};

#endif