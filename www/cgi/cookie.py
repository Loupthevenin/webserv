#!/usr/bin/env python3

import cgi
import os
import http.cookies
import uuid
# import time

sessions = {}


def get_session_id():
    cookie = http.cookies.SimpleCookie(os.environ.get("HTTP_COOKIE", ""))
    session_id_cookie = cookie.get("session_id")

    if session_id_cookie and session_id_cookie.value in sessions:
        return session_id_cookie.value
    else:
        return None


def set_session_cookie(session_id):
    cookie = http.cookies.SimpleCookie()
    cookie["session_id"] = session_id
    cookie["session_id"]["path"] = "/"
    print(cookie)


def delete_session_cookie():
    cookie = http.cookies.SimpleCookie()
    cookie["session_id"] = ""
    cookie["session_id"]["path"] = "/"
    print(cookie)


print("HTTP/1.1 200 OK")
print("Content-type: text/html")
print()

form = cgi.FieldStorage()
action = form.getvalue("action")

if action == "enable":
    session_id = get_session_id()
    if session_id is None:
        session_id = str(uuid.uuid4())
        sessions[session_id] = {"visits": 1}
        set_session_cookie(session_id)
        print(f"<p>Votre ID de session est : {session_id}</p>")

elif action == "disable":
    delete_session_cookie()

else:
    session_id = get_session_id()
    if session_id:
        sessions[session_id]["visits"] += 1
        visit_count = sessions[session_id]["visits"]
        print(
            f"<p>Vous avez visité cette page {visit_count} fois dans cette session.</p>"
        )
