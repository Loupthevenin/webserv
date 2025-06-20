# Webserv

## 🌐 Objective

The purpose of this project is to build a fully functional HTTP/1.1 server in C++98. You will implement a non-blocking web server capable of handling multiple clients, serving static content, executing CGI scripts, and responding correctly to GET, POST, and DELETE HTTP requests.

## 🧠 Skills Involved

- Network programming (sockets, epoll)
- Non-blocking I/O and multiplexing
- HTTP protocol and status codes
- File and directory handling
- Process and signal management
- Configuration parsing
- C++ OOP in a constrained (C++98) environment
- CGI handling

## 🚀 Features

### Mandatory

- **HTTP 1.1 Compliance**:
  - Correct handling of headers and response codes.
  - Persistent connections (keep-alive).
- **Non-blocking I/O** using `epoll()`:
  - Only one instance of the I/O multiplexer should handle all clients.
- **Configuration file support**:
  - Define servers, host/port bindings, root directories, error pages, body size limits, and route settings.
- **Static File Serving**
- **HTTP Methods**:
  - GET
  - POST
  - DELETE
- **Directory Listing (Autoindex)**
- **File Upload Support**
- **Default and Custom Error Pages**
- **Multiple virtual servers** on different ports or hostnames
- **CGI Execution**:
  - Based on file extension (e.g., `.sh`, `.py`)
  - Correct environment variable handling
  - Must support GET and POST
- **Browser Compatibility**:
  - Must be accessible and functional via a real browser.

### Bonus

- Cookie and session handling
- Support for multiple CGI interpreters

## 🛠️ Usage

```bash
make            # Build the server

./webserv [config_file]   # Run the server with a config file
```
