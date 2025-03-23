#!/usr/bin/bash

BODY="<html>
<head><title>CGI Bash</title></head>
<body>
<h1>Hello, CGI World!</h1>
<p>Ce script fonctionne via CGI en HTTP/1.1 !</p>
</body>
</html>"

CONTENT_LENGTH=${#BODY}

echo -e "HTTP/1.1 200 OK\r\n"
echo -e "Content-Type: text/html\r\n"
echo -e "Content-Length: $CONTENT_LENGTH\r\n"
echo -e "Connection: close\r\n"
echo -e "\r\n"

echo -e "$BODY"

# echo -e "<h2>Variables d'environnement</h2>"
# echo -e "<pre>"
# env
# echo -e "</pre>"
# echo -e "</body>"
# echo -e "</html>"
