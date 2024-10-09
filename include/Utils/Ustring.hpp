#pragma once
#ifndef USTRING_HPP
# define USTRING_HPP

#include <vector>
#include <stdint.h>
#include <string>

class Ustring
{
private:
	std::vector<uint8_t> content;

public:
    // Default constructor
    Ustring() : content() {}

    // Constructor that initializes from a standard string
    Ustring(const std::string& str) : content(str.begin(), str.end()) {}

    // Constructor that initializes with a specified number of uint8_t elements
    Ustring(size_t numElements) : content(numElements) {}

	/// @brief Constructs a Ustring object from a standard string.
	/// @param str The string to initialize the Ustring with.
	Ustring(std::string& str) : content(str.begin(), str.end()) {}

	/// @brief Overloads the += operator to append a string to the Ustring content.
    /// @param str The string to append.
    /// @return A reference to the modified Ustring object.
    Ustring& operator+=(const std::string& str);

    /// @brief Overloads the += operator to append another Ustring to this Ustring content.
    /// @param other The Ustring object to append.
    /// @return A reference to the modified Ustring object.
    Ustring& operator+=(const Ustring& other);

    /// @brief Overloads the [] operator to access content at the specified index.
    /// @param index The index to access.
    /// @return A reference to the element at the specified index.
    /// @exception std::out_of_range Thrown if the index is out of bounds.
    uint8_t& operator[](size_t index);

    /// @brief Overloads the [] operator to access content at the specified index (const version).
    /// @param index The index to access.
    /// @return A const reference to the element at the specified index.
    /// @exception std::out_of_range Thrown if the index is out of bounds.
    const uint8_t& operator[](size_t index) const;

	friend std::ostream& operator<<(std::ostream& os, const Ustring& msg);

	/// @brief Finds the first occurrence of a substring in the Ustring content.
	/// @param str The Ustring object to search within.
	/// @param substr The substring to search for.
	/// @return The starting position of the substring if found, otherwise `std::string::npos`.
	size_t find(const std::string &substr, size_t pos = 0) const;

  	/// @brief Extracts a substring from the Ustring content.
	/// @param vec The Ustring object to extract the substring from.
	/// @param pos The starting position of the substring.
	/// @param len The length of the substring to extract.
	/// @exception std::out_of_range Thrown if the starting position is out of range.
	/// @return A vector of uint8_t containing the extracted substring.
	Ustring substr(size_t pos, size_t len) const;

	/// @brief Extracts a substring from the starting position to the end of the Ustring content.
	/// @param vec The Ustring object to extract the substring from.
	/// @param startPos The starting position of the substring.
	/// @exception std::out_of_range Thrown if the starting position is out of range.
	/// @return A vector of uint8_t containing the extracted substring.
	Ustring substr(size_t startPos) const;

	/// @brief Returns the size of the Ustring content.
	/// @return The size (number of bytes) of the content.
	size_t size() const { return content.size(); }

   	/// @brief Checks if the Ustring content is empty.
	/// @return True if the content is empty, otherwise false.
	bool empty() const { return content.empty(); }

	/// @brief Provides access to the underlying content of the Ustring.
	/// @return A const reference to the internal vector of uint8_t representing the content.
	const std::vector<uint8_t>& get_content() const { return content; }

	/// @brief Reserves space in the Ustring's internal vector.
	/// @param n The number of elements to reserve space for.
	void reserve(size_t n) { content.reserve(n); }

	/// @brief Adds a single uint8_t element to the end of the Ustring content.
	/// @param value The value to be added.
	void push_back(uint8_t value) { content.push_back(value); }

	// Returns a pointer to the underlying array of uint8_t elements
    uint8_t* data() { return content.data(); }

    // Returns a const pointer to the underlying array of uint8_t elements
    const uint8_t* data() const { return content.data(); }

    // Converts the Ustring content back into a standard string
   	std::string toString() const { return std::string(content.begin(), content.end()); }


    /// @brief Erases the element at the specified position.
    /// @param pos The position of the element to erase.
    /// @return A reference to the modified Ustring object.
	Ustring& erase(size_t pos);

    /// @brief Erases a range of elements from the content.
    /// @param pos The starting position of the range.
    /// @param len The number of elements to erase.
    /// @return A reference to the modified Ustring object.
	Ustring& erase(size_t pos, size_t len);

	/// @brief Finds the first element that is not any of the characters in the given set.
    /// @param str A string containing the set of characters to avoid.
    /// @param pos The starting position for the search.
    /// @return The position of the first element not in the set, or std::string::npos if not found.
    size_t find_first_not_of(const std::string& str, size_t pos = 0) const;
};

#endif