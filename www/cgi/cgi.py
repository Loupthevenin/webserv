#!/usr/bin/env python3

import cgi
import os


UPLOAD_DIR = "www/uploads/"


def handle_upload():
    form = cgi.FieldStorage()

    if "file" not in form:
        print("Content-type: text/html\n")
        print("<h1>Aucun fichier sélectionné !</h1>")
        return

    file_item = form["file"]
    if file_item.filename:
        filename = os.path.basename(file_item.filename)
        filepath = os.path.join(UPLOAD_DIR, filename)

        with open(filepath, "wb") as output_file:
            output_file.write(file_item.file.read())

        print("Content-type: text/html\n")
        print("<html><body>")
        print(f"<h1>Le fichier {filename} a été téléversé avec succés !</h1>")
        print(f"<p>Fichier sauvegardé dans {filepath}</p>")
        print("</body></html>")
    else:
        print("Content-type: text/html\n")
        print("<h1>Le fichier n'a pas été téléversé correctement.</h1>")


handle_upload()
