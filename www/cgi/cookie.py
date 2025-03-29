#!/usr/bin/env python3

import warnings

warnings.filterwarnings("ignore", category=DeprecationWarning)

import cgi
import os
import json
import http.cookies
import uuid


def get_session_id():
    cookie = http.cookies.SimpleCookie(os.environ.get("HTTP_COOKIE", ""))
    session_id_cookie = cookie.get("session_id")
    return session_id_cookie.value if session_id_cookie else None


def get_visit_count(session_id):
    cookie_header = os.environ.get("HTTP_COOKIE", "")
    cookie = http.cookies.SimpleCookie(cookie_header)
    visit_cookie = cookie.get(f"visits_{session_id}")
    return int(visit_cookie.value) if visit_cookie else 0


def set_session_cookie(session_id, visits):
    cookie = http.cookies.SimpleCookie()
    cookie["session_id"] = session_id
    cookie["session_id"]["path"] = "/"
    cookie[f"visits_{session_id}"] = visits
    print(cookie.output())


print("HTTP/1.1 200 OK")
print("Content-type: application/json")

try:
    form = cgi.FieldStorage()
    action = form.getvalue("action")
except Exception:
    action = None

response = {}

if action == "enable":
    session_id = get_session_id()
    if session_id is None:
        session_id = str(uuid.uuid4())
        set_session_cookie(session_id, 0)
    response["status"] = "enabled"
else:
    session_id = get_session_id()
    if session_id:
        visits = get_visit_count(session_id) + 1
        set_session_cookie(session_id, visits)
        response["visits"] = visits

print()
print(json.dumps(response))
