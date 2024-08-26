#include <Port.hpp>
#include <StringUtils.hpp>

void Port::InitPort(int _port)
{
	if (_port < 1 || _port > 65535)
		throw InvalidPort();
	else
		this->port = _port;
}

void Port::InitPort(std::string portString)
{
	int _port = StringUtils::StrintToInt(portString);

	if (_port < 1 || _port > 65535)
		throw InvalidPort();

	port = _port;
}

bool Port::operator==(const Port &p)
{
	return (p.port == this->port);
}
