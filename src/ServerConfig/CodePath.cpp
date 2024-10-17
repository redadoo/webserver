#include "CodePath.hpp"

std::ostream &operator<<(std::ostream &os, const CodePath &cp)
{
	os << "Code: " << cp.code << ", Path: " << cp.path;
	return os;
}

CodePath::CodePath(Code _code, const std::string &_path)
{
	this->code = _code;
	this->path = _path;
}

CodePath::CodePath(int _code, const std::string &_path)
{
	this->code = static_cast<Code>(_code);
	this->path = _path;
}

bool CodePath::isSet() const
{
	return !path.empty();
}
