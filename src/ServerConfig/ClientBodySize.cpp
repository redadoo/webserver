#include "ClientBodySize.hpp"
#include <StringUtils.hpp>

std::ostream &operator<<(std::ostream &os, const ClientBodySize &cl)
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

void ClientBodySize::InitClientBodySize(const std::string& str)
{
	char lastChar = str[str.size() - 1];

	if (!(lastChar == 'K' || lastChar == 'M' || lastChar == 'G'))
	{
		if(!StringUtils::IsAllDigit(str))
				throw InvalidClientBodySize();

		this->size = str;
		this->unit =  BYTE;
	}
	else
	{
		std::string size = str.substr(0, str.size() - 1);

		if (size.size() == 0)
			throw InvalidClientBodySize();

		if(!StringUtils::IsAllDigit(size))
			throw InvalidClientBodySize();

		this->size = size;

		if (lastChar == 'K')
			this->unit = KILOBYTE;
		else if (lastChar == 'M')
			this->unit = MEGABYTE;
		else if (lastChar == 'G')
			this->unit = GIGABYTE;
	}
}
