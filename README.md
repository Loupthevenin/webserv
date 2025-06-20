# Webserv

## 🌐 Objective

Webserv is an HTTP/1.1 server that I developed in C++98. The goal of this project was to better understand how modern web servers work under the hood by implementing one from scratch. My server is non-blocking, event-driven, and capable of handling multiple simultaneous clients efficiently using `epoll()`.

It supports static file serving, basic routing, CGI execution, file uploads, and custom error pages—all configured through a flexible configuration file.

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
