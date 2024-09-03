#!/usr/bin/env python3
import os
import sys
import urllib.parse

def generate_html(params):
    params_html = "\n".join(f"<li><strong>{key}:</strong> {value[0]}</li>" for key, value in params.items())

    html = f"""Content-Type: text/html

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>CGI Parameter Demo</title>
    <style>
        body {{
            font-family: Arial, sans-serif;
            line-height: 1.6;
            padding: 20px;
            max-width: 800px;
            margin: 0 auto;
            background-color: #f4f4f4;
        }}
        .container {{
            background-color: white;
            padding: 20px;
            border-radius: 5px;
            box-shadow: 0 0 10px rgba(0,0,0,0.1);
        }}
        h1 {{
            color: #333;
        }}
        ul {{
            list-style-type: none;
            padding: 0;
        }}
        li {{
            margin-bottom: 10px;
            padding: 10px;
            background-color: #f0f0f0;
            border-radius: 3px;
        }}
    </style>
</head>
<body>
    <div class="container">
        <h1>CGI Parameter Demo</h1>
        <p>Here are all the parameters received:</p>
        <ul>
            {params_html}
        </ul>
        <p>Environment Variables:</p>
        <ul>
            <li><strong>REQUEST_METHOD:</strong> {os.environ.get('REQUEST_METHOD', 'Not set')}</li>
            <li><strong>QUERY_STRING:</strong> {os.environ.get('QUERY_STRING', 'Not set')}</li>
            <li><strong>CONTENT_LENGTH:</strong> {os.environ.get('CONTENT_LENGTH', 'Not set')}</li>
            <li><strong>CONTENT_TYPE:</strong> {os.environ.get('CONTENT_TYPE', 'Not set')}</li>
            <li><strong>SCRIPT_NAME:</strong> {os.environ.get('SCRIPT_NAME', 'Not set')}</li>
            <li><strong>PATH_INFO:</strong> {os.environ.get('PATH_INFO', 'Not set')}</li>
        </ul>
    </div>
</body>
</html>"""
    return html

# Ottieni i parametri dalla query string
query_string = os.environ.get('QUERY_STRING', '')
params = urllib.parse.parse_qs(query_string)

# Se è una richiesta POST, leggi anche il body
if os.environ.get('REQUEST_METHOD') == 'POST':
    content_length = int(os.environ.get('CONTENT_LENGTH', 0))
    post_data = sys.stdin.read(content_length)
    post_params = urllib.parse.parse_qs(post_data)
    params.update(post_params)

# Genera e stampa l'HTML
print(generate_html(params))
sys.stdout.flush()
