#include <ClientBodySize.hpp>
#include <StringUtils.hpp>
#include <limits>
#include <sstream>

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
			throw std::invalid_argument("Invalid client body size");

		this->size = StringUtils::StringToUnsignedLongLong(str);
		this->unit =  BYTE;
	}
	else
	{
		std::string size = str.substr(0, str.size() - 1);

		if (size.size() == 0)
			throw std::invalid_argument("Invalid client body size");

		if(!StringUtils::IsAllDigit(size))
			throw std::invalid_argument("Invalid client body size");

		this->size = StringUtils::StringToUnsignedLongLong(size);

		if (lastChar == 'K')
			this->unit = KILOBYTE;
		else if (lastChar == 'M')
			this->unit = MEGABYTE;
		else if (lastChar == 'G')
			this->unit = GIGABYTE;
	}
}

unsigned long long ClientBodySize::ConvertToBytes() const
{
	switch (unit)
	{
		case BYTE:
			return size;
		case KILOBYTE:
			if (size > std::numeric_limits<unsigned long long>::max() / 1024)
				throw std::invalid_argument("Client body size is too large");
			return size * 1024;
		case MEGABYTE:
			if (size > std::numeric_limits<unsigned long long>::max() / 1024 / 1024)
				throw std::invalid_argument("Client body size is too large");
			return size * 1024 * 1024;
		case GIGABYTE:
			if (size > std::numeric_limits<unsigned long long>::max() / 1024 / 1024 / 1024)
				throw std::invalid_argument("Client body size is too large");
			return size * 1024 * 1024 * 1024;
		default:
			throw std::invalid_argument("Invalid client body size unit");
	}
}
