#!/usr/bin/bash

CONTENT_LENGTH=${#BODY}

echo -e "HTTP/1.1 200 OK\r\n"
# echo -e "Content-Type: text/html\r\n"
# echo ""

REQUEST_METHOD=$(echo "$REQUEST_METHOD")

echo "<html><body>"

if [ "$REQUEST_METHOD" = "GET" ]; then
	echo "<h2>Requête GET</h2>"
	echo "<p>Query String: $QUERY_STRING</p>"

	IFS='&' read -ra PARAMS <<<"$QUERY_STRING"
	for param in "${PARAMS[@]}"; do
		key=$(echo "$param" | cut -d '=' -f 1)
		value=$(echo "$param" | cut -d '=' -f 2)
		echo "<p><b>$key</b>: $value</p>"
	done

elif [ "$REQUEST_METHOD" = "POST" ]; then
	echo "<h2>Requête POST</h2>"

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
	echo "<h2>Méthode non supportée</h2>"
fi

echo "</body></html>"
