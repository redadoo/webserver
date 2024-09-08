#!/usr/bin/env python3
import os
import cgi

def main():
    print("Content-Type: text/html\n")
    print("<html><head><title>CGI Test</title></head><body>")
    print("<h1>CGI Test Script</h1>")

    # Print server information
    print("<h2>Server Information:</h2>")
    print("<p>Server Software: {}</p>".format(os.environ.get('SERVER_SOFTWARE', 'Not provided')))
    print("<p>Server Name: {}</p>".format(os.environ.get('SERVER_NAME', 'Not provided')))
    print("<p>Gateway Interface: {}</p>".format(os.environ.get('GATEWAY_INTERFACE', 'Not provided')))

    # Print request information
    print("<h2>Request Information:</h2>")
    print("<p>Request Method: {}</p>".format(os.environ.get('REQUEST_METHOD', 'Not provided')))
    print("<p>Path Info: {}</p>".format(os.environ.get('PATH_INFO', 'Not provided')))
    print("<p>Script Name: {}</p>".format(os.environ.get('SCRIPT_NAME', 'Not provided')))

    # Handle GET request
    if os.environ.get('REQUEST_METHOD') == 'GET':
        print("<h2>GET Parameters:</h2>")
        query_string = os.environ.get('QUERY_STRING', '')
        if query_string:
            params = cgi.parse_qs(query_string)
            for key, value in params.items():
                print("<p>{}: {}</p>".format(key, value[0]))
        else:
            print("<p>No GET parameters provided.</p>")

    # Handle POST request
    elif os.environ.get('REQUEST_METHOD') == 'POST':
        print("<h2>POST Data:</h2>")
        form = cgi.FieldStorage()
        for key in form.keys():
            print("<p>{}: {}</p>".format(key, form.getvalue(key)))



    print("</body></html>")

if __name__ == "__main__":
    main()
