#pragma once
#ifndef SERVER_HPP
# define SERVER_HPP

#include <Client.hpp>
#include <sys/epoll.h>
#include <HttpResponse.hpp>

#define MAX_RESPONSE_CHUNK_SIZE 4096
#define MAX_REDIRECT 5

/// @brief Class representing server information and its operations.
class Server
{
private:
	/// @brief Sends an HTTP redirect response to the client with the specified status code and redirection path, then processes the redirected request.
	/// @param client The client to which the redirect response will be sent.
	/// @param redirect Contains the HTTP status code and the redirection path.
	/// @param redirectCount The current count of redirects, used to prevent infinite redirection loops.
	void SendRedirectResponse(Client& client, const CodePath& redirect, int redirectCount);


	/// @brief Logs all the response headers to the server log, including both header names and their values.
	void LogResponseHeaders();

	/// @brief Handles requests for individual files by reading the file content and setting the appropriate response status and content type.
	/// @param path The filesystem path of the file to be served.
	void HandleFileRequest(const std::string& path);

	/// @brief Handles requests for directories by searching for an index file within the directory.
	/// @param path The filesystem path of the directory to check for an index file.
	void HandleDirectoryRequest(const std::string& path);

	/// @brief Handles the request for directory listing by generating an HTML page that lists the contents of the specified directory.
	/// @param path The filesystem path of the directory to list.
	/// @param client The client requesting the directory listing.
	void HandleDirectoryListing(const std::string& path, Client& client);

	/// @brief Handles file upload requests from the client by processing multipart/form-data and saving the uploaded files to the configured upload directory.
	/// @param client The client making the upload request.
	/// @param location The location object that contains the configuration and permissions for the requested resource.
	void HandleUploadRequest(Client& client, const Location* location);

	/// @brief Handles the GET HTTP request from the client, determining the appropriate action based on the requested resource.
	/// @param client The client making the GET request.
	/// @param location The location object that contains the configuration and permissions for the requested resource.
	void HandleGetRequest(Client& client, const Location* location);

	/// @brief Handles the POST HTTP request from the client, determining the appropriate action based on the location's configuration.
	/// @param client The client making the POST request.
	/// @param location The location object that contains the configuration and permissions for the requested resource.
	void HandlePostRequest(Client& client, const Location* location);

	/// @brief Handles the DELETE HTTP request from the client, checking if the DELETE method is allowed for the given location and attempting to delete the specified file.
	/// @param client The client making the DELETE request.
	/// @param location The location object that contains the configuration and permissions for the requested resource.
	void HandleDeleteRequest(const Client& client, const Location* location);

	/// @brief Adds a new client to the server's client list.
	/// @param clientFd File descriptor for the client's socket.
	/// @param ip Source IP address of the client.
	/// @param port Source port of the client.
	void AddClient(int clientFd, std::string ip, uint16_t port);

public:
	ServerConfig					serverConfig;
	HttpResponse					response;
	std::map<int, Client>			clients;
	int								serverFd;

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
	void ReadClientRequest(Client& client);

	/// @brief Processes the client's request and generates a response.
	/// @param client Reference to the Client object.
	void ProcessRequest(Client& client, int redirectCount);

	/// @brief Sends an error response to the client with the specified HTTP status code, including an error message body based on the server configuration.
	/// @param client The client to which the error response will be sent.
	/// @param code The HTTP status code representing the error condition.
	void SendErrorResponse(const Client& client, HttpStatusCode::Code code);

	bool IsCgiRequest(const std::string& path, const Location* location) const;
	void HandleCgiRequest(Client& client, const std::string& scriptPath, const Location* location);

	/// @brief Closes the connection to a client and removes it from the server's client list.
	/// @param client Reference to the Client object.
	void CloseClientConnection(const Client& client);

	/// @brief Closes the connection to a client and removes it from the server's client list.
	/// @param clientFd File descriptor of the client.
	void CloseClientConnection(int clientFd);

	/// @brief Sends a response to the client.
	/// @param client Reference to the Client object.
	void SendResponse(const Client& client);
	/// @brief Overloads the << operator to output server configuration details.
	/// @param os Output stream object.
	/// @param sr Reference to the Server object.
	/// @return Reference to the output stream object.
	friend std::ostream& operator<<(std::ostream& os, const Server& sr);
};

#endif // SERVER_HPP

