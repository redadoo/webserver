
#ifndef CLIENTBODYSIZE_HPP
# define CLIENTBODYSIZE_HPP

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
};

#endif