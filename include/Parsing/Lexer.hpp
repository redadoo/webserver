#ifndef LEXER_HPP
# define LEXER_HPP

# include <string>
# include <vector>
# include <fstream>

namespace Lexer
{

	enum ParseState 
	{
		SearchingServer,
		SearchingOpenBracket,
		CollectingTokenName,
		CollectingTokenValue,
		CollectionLocation,
	};
		
	enum TokenType
	{
		startServerContext,
		endServerContext,
		startLocationContext,
		endLocationContext,
		simpleToken
	};

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

	std::vector<Token> GetToken(std::string fileName);

	class FileNotFound : public std::exception {
		public:
			virtual const char* what() const throw() { return "Error: file not found";}
	};

}

#endif