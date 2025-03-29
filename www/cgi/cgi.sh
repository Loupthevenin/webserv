#!/usr/bin/bash

REQUEST_METHOD=$(echo "$REQUEST_METHOD")

POST_DATA=""

if [ "$REQUEST_METHOD" = "POST" ]; then
	read -r POST_DATA
fi

BODY="<html>
<head>
<meta charset='UTF-8'>
<title>CGI Bash</title>
<style>
body {
            font-family: Arial, sans-serif;
            background-color: #f4f4f9;
            color: #333;
            text-align: center;
            margin: 0;
            padding: 0;
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            min-height: 100vh;
        }

        h1 {
            color: #007bff;
            font-size: 2.5rem;
            margin-bottom: 10px;
        }

        p {
            font-size: 1.2rem;
            background: white;
            padding: 15px;
            border-radius: 8px;
            box-shadow: 0px 4px 8px rgba(0, 0, 0, 0.2);
            max-width: 500px;
            margin: 10px auto;
        }

        strong {
            color: #ff5733;
        }

        p:hover {
            transform: scale(1.05);
            transition: 0.3s ease-in-out;
        }
</style>
</head>
<body>
<h1>Hello, CGI World!</h1>
<p>Ce script fonctionne via CGI en HTTP/1.1 !</p>
<p><strong>Methode HTTP : </strong>$REQUEST_METHOD</p>"

if [ "$REQUEST_METHOD" = "POST" ]; then
	BODY+="<p><strong>Données POST: </strong>$POST_DATA</p>"
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
