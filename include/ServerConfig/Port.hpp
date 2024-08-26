#ifndef PORT_HPP
#define PORT_HPP

#include <stdint.h>
#include <string>

class Port
{
public:
	uint16_t port;

	Port() : port(0) {};

	void InitPort(int _port);
	void InitPort(std::string _port);

	bool operator == (const Port &p);
	 
	class InvalidPort : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Invalid port";
			}
	};
};

#endif