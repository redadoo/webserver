
#ifndef CLIENTBODYSIZE_HPP
# define CLIENTBODYSIZE_HPP

# include <iostream>
# include <string>

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
	Unit		unit;

	void InitClientBodySize(const std::string& str);
	
	friend std::ostream& operator<<(std::ostream& os, const ClientBodySize& cl);

	class InvalidClientBodySize : public std::exception
	{
		public:
			virtual const char *what() const throw()
			{
				return "Invalid client body size";
			}
	};
};

#endif