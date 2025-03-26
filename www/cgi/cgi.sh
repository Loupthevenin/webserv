#!/usr/bin/bash

REQUEST_METHOD=$(echo "$REQUEST_METHOD")

POST_DATA=""

if [ "$REQUEST_METHOD" = "POST" ]; then
	read -r POST_DATA
fi

BODY="<html>
<head><meta charset='UTF-8'><title>CGI Bash</title></head>
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

echo -e "HTTP/1.1 200 OK\r"
echo -e "Content-type: text/html; charset=UTF-8\r"
echo -e "\r"
# echo -e "Content-Length: $CONTENT_LENGTH\r\n"
# echo -e "Connection: close\r\n"
# echo -e "\r\n"

echo -e "$BODY"
