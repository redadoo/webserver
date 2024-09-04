#!/usr/bin/env python3
import cgi
import json
import os

print("Content-Type: text/html\r\n\r\n")

method = os.environ.get('REQUEST_METHOD')
if method == 'POST':
    if os.environ.get('CONTENT_TYPE') == 'application/json':
        length = int(os.environ.get('CONTENT_LENGTH', 0))
        data = json.loads(sys.stdin.read(length))
    else:
        form = cgi.FieldStorage()
        data = {key: form.getvalue(key) for key in form.keys()}

    print("<h1>Dati ricevuti:</h1>")
    for key, value in data.items():
        print(f"<p>{key}: {value}</p>")
else:
    print("<h1>Metodo non supportato</h1>")
