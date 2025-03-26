#!/usr/bin/env python3

import warnings

warnings.filterwarnings("ignore", category=DeprecationWarning)

import os
import cgi
import json

UPLOAD_DIR = "www/uploads/"


def handle_upload():
    form = cgi.FieldStorage()

    if "file" not in form:
        print("HTTP/1.1 200 OK\r\nContent-Type: application/json; charset=UTF-8\r\n")
        print(json.dumps({"error": "Aucun fichier sélectionner !"}))
        return

    file_item = form["file"]
    if file_item.filename:
        filename = os.path.basename(file_item.filename)
        filepath = os.path.join(UPLOAD_DIR, filename)

        try:
            with open(filepath, "wb") as output_file:
                output_file.write(file_item.file.read())

            print(
                "HTTP/1.1 200 OK\r\nContent-Type: application/json; charset=UTF-8\r\n"
            )
            response = {
                "success": True,
                "message": f"Le fichier {filename} a été téléversé avec succès !",
                "file_path": filepath,
            }
            print(json.dumps(response))
        except Exception as e:
            print(
                "HTTP/1.1 200 OK\r\nContent-Type: application/json; charset=UTF-8\r\n"
            )
            response = {
                "error": f"Erreur lors de l'enregistrement du fichier: {str(e)}"
            }
            print(json.dumps(response))
    else:
        print("Content-type: application/json; charset=UTF-8\n")
        response = {"error": "Le fichier n'a pas été téléversé correctement."}
        print(json.dumps(response))


handle_upload()
