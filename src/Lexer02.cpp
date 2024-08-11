#include <Lexer.hpp>

    enum ParseState {
        SearchingServer,
        SearchingOpenBracket,
        CollectingTokenName,
        CollectingTokenValue,
		CollectionLocation,
    };	
std::vector<Lexer::Token> Lexer::GetToken(std::string fileName)
{

	Logger::Log("start parsing Configuration file");
	std::string line;
	std::vector<Lexer::Token> tokens;
	std::string tempName;
	std::string tempValue;
	bool isLocationBlock = false;
	Lexer::Token token;
	if (!utils::CheckFileExistence(fileName.c_str()))
		throw WebServerExceptions::FileNotFound();
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
                    if (isspace(static_cast<unsigned char>(currentChar))) {
                        continue; // Skip whitespace
                    }

                    if (line.find("server") == i) {
                        state = SearchingOpenBracket;
                        i += 5; // Skip the "server" keyword
                    } else {
                        throw WebServerExceptions::ErrorOnFileConfigurationSyntax();
                    }
                    break;

                case SearchingOpenBracket:
                    if (isspace(static_cast<unsigned char>(currentChar))) {
                        continue; // Skip whitespace
                    }

                    if (currentChar == '{') {
                        state = CollectingTokenName;
                    } else {
                        throw WebServerExceptions::ErrorOnFileConfigurationSyntax();
                    }
                    break;

                case CollectingTokenName:
					if (currentChar == '}')
						{
							if(!isLocationBlock)
								state = SearchingServer;
                            else
                                isLocationBlock =  false;
							break;
						}
                    if (isspace(static_cast<unsigned char>(currentChar))) {
                        if (!tempName.empty()) {
							if (tempName == "location")
								state = CollectionLocation;
							else
	                            state = CollectingTokenValue;
                        }
                        continue; // Skip whitespace after collecting the name
                    }

                    if (isalpha(currentChar) || currentChar == '_' || currentChar == '/') {
                        tempName += currentChar;
                    } else {
                        
                        throw WebServerExceptions::ErrorOnFileConfigurationSyntax();
                    }
                    break;

                case CollectingTokenValue:

                    if (currentChar == ';') {
                        // End of token value, store the token
                        Lexer::Token token;
                        token.tokenName = tempName;
                        token.tokenValue = tempValue;
                        tokens.push_back(token);
                        std::cout << "::Token name: " << token.tokenName << " ::Token Value:" << token.tokenValue << std::endl;

                        // Clear temp variables and return to collecting token names
                        tempName.clear();
                        tempValue.clear();
                        state = CollectingTokenName;
                    } else if (isspace(static_cast<unsigned char>(currentChar))) {
                        if (!tempValue.empty() && tempValue[tempValue.size() - 1] != ' ') {
                            tempValue += ' ';  // Separate multiple values with a space
                        }
                    } else if (currentChar != '}') {
                        tempValue += currentChar;
                    } else {
                        throw WebServerExceptions::ErrorOnFileConfigurationSyntax();
                    }
                    break;
				case CollectionLocation:

					if (isspace(static_cast<unsigned char>(currentChar))) {
                        continue; // Skip whitespace
                    }

					if (currentChar == '{')
					{
                        isLocationBlock = true;
                        token.tokenName = tempName;
						token.tokenValue = tempValue;
                        tokens.push_back(token);
                        tempName.clear();
                        tempValue.clear();
						state = CollectingTokenName;
					} 
					else if (isalnum(currentChar) || currentChar == '/' || currentChar == '=')
					{
						tempValue += currentChar;
					}
					else	
						throw WebServerExceptions::ErrorOnFileConfigurationSyntax();
					break;

            }
        }
    }
	file.close();
	return (tokens);
}
