#!/usr/bin/env python3
import os
import sys

def main():
	print("Content-Type: text/html\n")
	print("<html><head><title>CGI Test</title></head><body>")
	print("<h1>CGI Test Script</h1>")

	# Print server information
	print("<h2>Server Information:</h2>")
	print("<p>Server Name: {}</p>".format(os.environ.get('SERVER_NAME', 'Not provided')))
	print("<p>Server Port: {}</p>".format(os.environ.get('SERVER_PORT', 'Not provided')))

	# Print request information
	print("<h2>Request Information:</h2>")
	print("<p>Request Method: {}</p>".format(os.environ.get('REQUEST_METHOD', 'Not provided')))
	print("<p>Path Info: {}</p>".format(os.environ.get('PATH_INFO', 'Not provided')))

	# Handle GET request
	if os.environ.get('REQUEST_METHOD') == 'GET':
		print("<h2>GET Parameters:</h2>")
		query_string = os.environ.get('QUERY_STRING', '')
		if query_string:
			for param in query_string.split('&'):
				key, value = param.split('=')
				print("<p>{}: {}</p>".format(key, value))
		else:
			print("<p>No GET parameters provided.</p>")

	# Handle POST request
	elif os.environ.get('REQUEST_METHOD') == 'POST':
		print("<h2>POST Data:</h2>")
		print("<p>Content Length: {}</p>".format(os.environ.get('CONTENT_LENGTH', 'Not provided')))
		print("<p>Content Type: {}</p>".format(os.environ.get('CONTENT_TYPE', 'Not provided')))

		size = int(os.environ.get('CONTENT_SIZE', 0))
		for i in range(size):
			print("<p>Content {}: ".format(i + 1), end='')
			print("{}</p>".format(os.environ.get('CONTENT_{}'.format(i + 1), 'Not provided')))



	print("</body></html>")

if __name__ == "__main__":
	main()
