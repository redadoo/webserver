# include <Lexer.hpp>
# include <Logger.hpp>
# include <Utils.hpp>
# include <WebServerException.hpp>

using namespace FIleUtils;

std::vector<Lexer::Token> Lexer::GetToken(std::string fileName)
{
	Logger::Log("start parsing Configuration file ...");

	std::string 				line;
	std::vector<Lexer::Token> 	tokens;
	std::string 				tempName;
	std::string 				tempValue;
	bool 						isLocationBlock = false;


	if (!CheckFileExistence(fileName.c_str()))
		throw WebServerException::FileNotFound();

	std::ifstream file(fileName.c_str());
	ParseState state = SearchingServer;

	while (std::getline(file, line))
	{
		for (size_t i = 0; i < line.size(); ++i)
		{
			char currentChar = line[i];

			if (currentChar == '#')
				break;

			switch (state)
			{
				case SearchingServer:

					if (isspace((currentChar)))
						continue; // Skip whitespace

					if (line.find("server") == i)
					{
						state = SearchingOpenBracket;
						i += 5; // Skip the "server" keyword
						tokens.push_back(Token("Server"));
					}
					else
						throw WebServerException::ErrorOnFileConfigurationSyntax();
					break;

				case SearchingOpenBracket:

					if (isspace((currentChar)))
						continue; // Skip whitespace

					if (currentChar == '{')
						state = CollectingTokenName;
					else
						throw WebServerException::ErrorOnFileConfigurationSyntax();
					break;

				case CollectingTokenName:
					if (currentChar == '}')
					{
						if(!isLocationBlock)
						{
							tokens.push_back(Token("ServerEnd"));
							state = SearchingServer;
						}
						else
						{
							tokens.push_back(Token("LocationEnd"));
							isLocationBlock =  false;
						}
						break;
					}
					if (isspace(static_cast<unsigned char>(currentChar)))
					{
						if (!tempName.empty())
						{
							if (tempName == "location")
								state = CollectionLocation;
							else
								state = CollectingTokenValue;
						}
						continue; // Skip whitespace after collecting the name
					}

					if (isalpha(currentChar) || currentChar == '_' || currentChar == '/')
						tempName += currentChar;
					else
						throw WebServerException::ErrorOnFileConfigurationSyntax();
					break;

				case CollectingTokenValue:

					if (currentChar == ';')
					{
						// End of token value, store the token
						tokens.push_back(Token(tempName, tempValue));
						// Clear temp variables and return to collecting token names
						tempName.clear();
						tempValue.clear();
						state = CollectingTokenName;
					}
					else if (isspace(static_cast<unsigned char>(currentChar)))
					{
						if (!tempValue.empty() && tempValue[tempValue.size() - 1] != ' ')
							tempValue += ' ';  // Separate multiple values with a space
					}
					else if (currentChar != '}')
						tempValue += currentChar;
					else
						throw WebServerException::ErrorOnFileConfigurationSyntax();
					break;
				case CollectionLocation:

					if (isspace(static_cast<unsigned char>(currentChar)))
						continue; // Skip whitespace

					if (currentChar == '{')
					{
						isLocationBlock = true;
						tokens.push_back( Token(tempName, tempValue));
						tempName.clear();
						tempValue.clear();
						state = CollectingTokenName;
					}
					else if (isalnum(currentChar) || currentChar == '/' || currentChar == '=')
						tempValue += currentChar;
					else
						throw WebServerException::ErrorOnFileConfigurationSyntax();
					break;

			}
		}
	}
	file.close();
	return (tokens);

}
