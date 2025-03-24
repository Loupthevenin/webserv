#!/usr/bin/env python3

import cgi
import os
import http.cookies
import uuid
import time

sessions = {}


def get_session_id():
    cookie = http.cookies.SimpleCookie(os.environ.get("HTTP_COOKIE", ""))
    session_id_cookie = cookie.get("session_id")

    if session_id_cookie is None or session_id_cookie not in sessions:
        session_id = str(uuid.uuid4())
        sessions[session_id] = {"visits": 0, "created_at": time.time()}
        return session_id
    else:
        return session_id_cookie.value


def set_session_cookie(session_id):
    cookie = http.cookies.SimpleCookie()
    cookie["session_id"] = session_id
    cookie["session_id"]["path"] = "/"
    print(cookie)


print("Content-type: text/html")
print()

session_id = get_session_id()

if session_id in sessions:
    sessions[session_id]["visits"] += 1

set_session_cookie(session_id)
visit_count = sessions[session_id]["visits"]
print(
    f"<p>Vous avez visité cette page {visit_count} fois dans cette session.</p>")
