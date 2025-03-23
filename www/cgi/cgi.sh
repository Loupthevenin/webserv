#!/usr/bin/bash

REQUEST_METHOD=$(echo "$REQUEST_METHOD")

POST_DATA=""

if [ "$REQUEST_METHOD" = "POST" ]; then
	read -r POST_DATA
fi

BODY="<html>
<head><title>CGI Bash</title></head>
<body>
<h1>Hello, CGI World!</h1>
<p>Ce script fonctionne via CGI en HTTP/1.1 !</p>
<p><strong>Methode HTTP :</strong>$REQUEST_METHOD</p>"

if [ "$REQUEST_METHOD" = "POST" ]; then
	BODY+="<p><strong>Données POST:</strong>$POST_DATA</p>"
fi

BODY+="</body>
</html>"

CONTENT_LENGTH=${#BODY}

echo -e "HTTP/1.1 200 OK\r\n"
# echo -e "Content-Type: text/html; charset=utf-8\r\n"
# echo -e "Content-Length: $CONTENT_LENGTH\r\n"
# echo -e "Connection: close\r\n"
# echo -e "\r\n"

echo -e "$BODY"
