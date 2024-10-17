#include <Port.hpp>
#include <StringUtils.hpp>
#include <iostream>

void Port::InitPort(std::string portString)
{
	int _port = StringUtils::StrintToInt(portString);

	if (_port < 1 || _port > 65535)
		throw std::invalid_argument("Invalid port");

	port = _port;
}

bool Port::operator==(const Port &p)
{
	return (p.port == this->port);
}

std::ostream &operator<<(std::ostream &os, const Port &pr)
{
	os << ":" << pr.port;
	return (os);
}
