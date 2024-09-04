#ifndef SERVER_HPP
#define SERVER_HPP

#include <ServerConfig.hpp>
#include <Client.hpp>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <map>
#include <vector>
#include <iostream>
#include <HttpResponse.hpp>

#define MAX_RESPONSE_SIZE 4096
#define MAX_REDIRECT 5

/// @brief Class representing server information and its operations.
class Server
{
private:

	/// @brief Adds a new client to the server's client list.
	/// @param clientFd File descriptor for the client's socket.
	/// @param ip Source IP address of the client.
	/// @param port Source port of the client.
	void AddClient(int clientFd, std::string ip, uint16_t port);

	/// @brief Builds the response to be sent to the client.
	void BuildResponse();

    /// @brief Constructs an error response to be sent to the client.
	void BuildErrorResponse();

public:
	ServerConfig                    serverConfig;
	HttpResponse					response;
	std::map<int, Client>           clients;
	int                             serverFd;

	Server();

	/// @brief Initializes the server socket and binds it to the specified address and port.
	/// @param epollFd File descriptor for the epoll instance to which the server socket is added.
	void InitSocket(int epollFd);

	/// @brief Checks if a client with the given file descriptor is connected to the server.
	/// @param clientFd File descriptor of the client to check.
	/// @return True if the client is connected, false otherwise.
	bool IsMyClient(int clientFd);

	/// @brief Retrieves the client object associated with the given file descriptor.
	/// @param clientFd File descriptor of the client.
	/// @return Reference to the Client object.
	Client& GetClient(int clientFd);

	/// @brief Accepts a new client connection and adds it to the server's client list.
	/// @param fd File descriptor of the server's listening socket.
	/// @param epollFd File descriptor for the epoll instance.
	/// @return 0 on success, -1 on failure.
	int AcceptClient(int fd, int epollFd);

	/// @brief Reads the client's response and stores it in the client's request.
	/// @param client Reference to the Client object.
	void ReadClientResponse(Client& client);

	/// @brief Processes the client's request and generates a response.
	/// @param client Reference to the Client object.
	void ProcessRequest(Client& client, int redirectCount);

	bool IsCgiRequest(const std::string& path, const Location* location) const;

	void HandleDeleteRequest(Client& client, const Location* location);
	void HandleGetRequest(Client& client, const Location* location);
	void HandlePostRequest(Client& client, const Location* location);
	void HandleCgiRequest(Client& client, const std::string& scriptPath, const Location* location);
	void HandleUploadRequest(Client& client, const Location* location);
	void HandleDirectoryListing(const std::string& path, Client& client);
	void HandleDirectoryRequest(const std::string& path);
	void HandleFileRequest(const std::string& path);
	void LogResponseHeaders();
	std::string GetFullPath(const std::string& path);
	std::string GetScriptPath(const std::string& path);
	std::pair<std::string, std::string> SplitPathAndQuery(const std::string& requestedPath);

	const Location* FindMatchingLocation(const std::string& requestPath) const;
	std::string GetBoundary(const std::string& contentType) const;
	std::vector<std::string> SplitMultipartData(const std::string& data, const std::string& boundary) const;
	std::string ExtractFilename(const std::string& part) const;
	std::string ExtractFileContent(const std::string& part) const;




	/// @brief Sends a response to the client.
	/// @param client Reference to the Client object.
	void SendResponse(const Client& client);

	void SendErrorResponse(const Client& client, HttpStatusCode::Code code);
	void SendRedirectResponse(Client& client, const CodePath& redirect, int redirectCount);


	/// @brief Closes the connection to a client and removes it from the server's client list.
	/// @param client Reference to the Client object.
	void CloseClientConnection(const Client& client);

	/// @brief Closes the connection to a client and removes it from the server's client list.
	/// @param clientFd File descriptor of the client.
	void CloseClientConnection(int clientFd);

    /// @brief Overloads the << operator to output server configuration details.
	/// @param os Output stream object.
	/// @param sr Reference to the Server object.
	/// @return Reference to the output stream object.
	friend std::ostream& operator<<(std::ostream& os, const Server& sr);
};

#endif // SERVER_HPP
