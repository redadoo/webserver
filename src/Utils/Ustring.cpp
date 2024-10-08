#include <Ustring.hpp>
#include <stdexcept>

Ustring& Ustring::operator+=(const std::string& str)
{
    for (size_t i = 0; i < str.size(); ++i)
        content.push_back(static_cast<uint8_t>(str[i]));

    return *this;
}

Ustring& Ustring::operator+=(const Ustring& other)
{
    content.insert(content.end(), other.get_content().begin(), other.get_content().end());
    return *this;
}

uint8_t& Ustring::operator[](size_t index)
{
    if (index >= content.size())
    {
        throw std::out_of_range("Index is out of range");
    }
    return content[index];
}

const uint8_t& Ustring::operator[](size_t index) const
{
    if (index >= content.size())
    {
        throw std::out_of_range("Index is out of range");
    }
    return content[index];
}

size_t Ustring::find(const std::string &substr, size_t pos) const
{
    if (substr.empty() || substr.size() > size()) 
    {
        return std::string::npos;
    }

    // Iterate over the vector and check for a match
    for (size_t i = pos; i <= size() - substr.size(); ++i) 
    {
        size_t j = 0;
		
        // Check if the substring matches from this position
        while (j < substr.size() && get_content()[i + j] == static_cast<uint8_t>(substr[j])) 
        {
            ++j;
        }

        // If we've matched the entire substring, return the starting index
        if (j == substr.size()) 
        {
            return i;
        }
    }
    
    return std::string::npos; 
}

Ustring Ustring::substr(size_t pos, size_t len) const
{
    if (pos >= size()) 
    {
        throw std::out_of_range("Position is out of range");
    }

    size_t end_pos = (pos + len > size()) ? size() - pos : len;
    Ustring result;
    result.reserve(end_pos);

    for (size_t i = 0; i < end_pos; ++i) 
    {
        result.push_back(get_content()[pos + i]);
    }

    return result;
}

Ustring Ustring::substr(size_t startPos) const
{
    if (startPos >= size()) 
    {
        throw std::out_of_range("Start position is out of range");
    }

    Ustring result;
    result.reserve(size() - startPos);

    for (size_t i = startPos; i < size(); ++i) 
    {
        result.push_back(get_content()[i]);
    }

    return result;
}

Ustring &Ustring::erase(size_t pos)
{
    if (pos >= content.size()) 
    {
        throw std::out_of_range("Position out of range");
    }
    content.erase(content.begin() + pos);
    return *this;
}

Ustring &Ustring::erase(size_t pos, size_t len)
{
    if (pos >= content.size() || pos + len > content.size()) 
    {
        throw std::out_of_range("Range out of bounds");
    }
    content.erase(content.begin() + pos, content.begin() + pos + len);
    return *this;
}

size_t Ustring::find_first_not_of(const std::string &str, size_t pos) const
{
    if (pos >= content.size()) 
        return std::string::npos;

    for (size_t i = pos; i < content.size(); ++i) 
    {
        if (str.find(content[i]) == std::string::npos) 
        {
            return i;
        }
    }

    return std::string::npos;
}

std::ostream &operator<<(std::ostream &os, const Ustring &msg)
{
    for (size_t i = 0; i < msg.content.size(); i++)
        os << msg.content[i];

    return os;    
}
