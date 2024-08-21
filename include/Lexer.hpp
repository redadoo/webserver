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
		locationContext,
		simpleToken
	};

	struct Token
	{
		TokenType tokenType;
		std::string tokenName;
		std::string tokenValue;

		Token(std::string _tokenName) : 
			tokenName(_tokenName) {};

		Token(std::string _tokenName, std::string _tokenValue) : 
			tokenName(_tokenName), tokenValue(_tokenValue) {};
	};

	std::vector<Token> GetToken(std::string fileName);

	class FileNotFound : public std::exception {
		public:
			virtual const char* what() const throw() { return "Error: file not found";}
	};

	class ErrorOnFileConfigurationSyntax : public std::exception {
		public:
			virtual const char* what() const throw() { return "Error: when try to parse configuration file";}
	};
}

#endif