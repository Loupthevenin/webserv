#!/usr/bin/bash

CONTENT_LENGTH=${#BODY}

echo -e "HTTP/1.1 200 OK\r\n"
# echo -e "Content-Type: text/html\r\n"
# echo ""

REQUEST_METHOD=$(echo "$REQUEST_METHOD")

HEAD="<html>
<head>
<meta charset='UTF-8'>
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
<body>"

echo -e "$HEAD"

if [ "$REQUEST_METHOD" = "GET" ]; then
	echo "<h1>Request GET</h1>"
	echo "<p>Query String: $QUERY_STRING</p>"

	IFS='&' read -ra PARAMS <<<"$QUERY_STRING"
	for param in "${PARAMS[@]}"; do
		key=$(echo "$param" | cut -d '=' -f 1)
		value=$(echo "$param" | cut -d '=' -f 2)
		echo "<p><b>$key</b>: $value</p>"
	done

elif [ "$REQUEST_METHOD" = "POST" ]; then
	echo "<h1>Request POST</h1>"

	if [ -z "$CONTENT_LENGTH" ]; then
		echo "<p>Aucune donnée reçue.</p>"
	else
		read -n "$CONTENT_LENGTH" POST_DATA
		echo "<p>Données reçues: $POST_DATA</p>"

		IFS='&' read -ra PARAMS <<<"$POST_DATA"
		for param in "${PARAMS[@]}"; do
			key=$(echo "$param" | cut -d '=' -f 1)
			value=$(echo "$param" | cut -d '=' -f 2)
			echo "<p><b>$key</b>: $value</p>"
		done
	fi
else
	echo "<h1>Méthode non supportée</h1>"
fi

echo "</body></html>"
