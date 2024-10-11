#pragma once
#ifndef LEXER_HPP
# define LEXER_HPP

#include <string>
#include <vector>

namespace Lexer
{
    /// @brief Enumeration for different parsing states during tokenization.
	enum ParseState 
	{
		SearchingServer,
		SearchingOpenBracket,
		CollectingTokenName,
		CollectingTokenValue,
		CollectionLocation,
	};

    /// @brief Enumeration for different token types.
	enum TokenType
	{
		startServerContext,
		endServerContext,
		startLocationContext,
		endLocationContext,
		simpleToken
	};

    /// @brief Structure representing a token with a name, value, and type.
	struct Token
	{
		std::string tokenName;
		std::string tokenValue;
		TokenType tokenType;

		Token(std::string _tokenName, TokenType _tokenType) : 
			tokenName(_tokenName), tokenType(_tokenType) {};

		Token(std::string _tokenName, std::string _tokenValue, TokenType _tokenType) : 
			tokenName(_tokenName), tokenValue(_tokenValue), tokenType(_tokenType) {};
	};

    /// @brief Tokenizes the input file into a vector of tokens.
    /// @param fileName The name of the file to tokenize.
    /// @return A vector of tokens parsed from the file.
	std::vector<Token> GetToken(std::string fileName);
}

#endif