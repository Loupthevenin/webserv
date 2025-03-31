/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_handleRequest.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdi-bia <opdi-bia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 14:57:19 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/31 21:09:09 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webserv.h"
#include <netinet/in.h>
#include <sys/socket.h>

void sendError(int fd, int statusCode) {
  HttpResponse response;
  response.setStatus(statusCode);
  response.setBody(buildErrorResponse(statusCode));

  std::string message = response.toString();
  send(fd, message.c_str(), message.size(), 0);
}

void closeConnexion(int fd, Epoll &epoll,
                    std::map<int, HttpRequest> &requests) {
  requests.erase(fd);
  epoll.removeFd(fd);
  close(fd);
}

std::string readFile(const std::string &filePath) {
  std::ifstream file(filePath.c_str());
  if (!file.is_open())
    return "404";

  std::string content;
  std::string line;
  while (std::getline(file, line))
    content += line + "\n";
  file.close();
  return content;
}

void setFdNonBlocking(int fd) {
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags == -1) {
    perror("fcntl(F_GETFL) failed");
    return;
  }
  if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
    perror("fcntl(O_NONBLOCK) failed");
    return;
  }
}

int check_file(std::string &filePath) {
  struct stat s;
  if (stat(filePath.c_str(), &s) != 0)
    return (404);
  if (S_ISDIR(s.st_mode))
    return (403);
  if (access(filePath.c_str(), R_OK) != 0)
    return (403);
  std::ifstream file(filePath.c_str(), std::ios::in | std::ios::binary);
  if (!file.is_open())
    return (500);
  file.close();
  return (200);
}

void check_dir(HttpResponse &response, std::string file) {
  struct stat s;

  size_t found = file.find_last_of("/");
  std::string dirPath = file.substr(0, found);
  if (stat(dirPath.c_str(), &s) != 0) {
    response.setStatus(404);
    response.setHeader("Content-Type", "text/plain");
    response.setBody("404 - Not Found");
  } else if (!S_ISDIR(s.st_mode)) {
    response.setStatus(403);
    response.setHeader("Content-Type", "text/plain");
    response.setBody("403 - Forbidden");
  } else if (access(dirPath.c_str(), W_OK | X_OK) != 0) {
    response.setStatus(403);
    response.setHeader("Content-Type", "text/plain");
    response.setBody("403 - Not Found");
  } else if (access(file.c_str(), F_OK) == -1) {
    response.setStatus(404);
    response.setHeader("Content-Type", "text/plain");
    response.setBody("404 - Not Found");
  } else if (std::remove(file.c_str()) == 0) {
    response.setStatus(200);
    response.setHeader("Content-Type", "text/plain");
    response.setBody("200 - file deleted");
  } else {
    response.setStatus(405);
    response.setHeader("Content-Type", "text/plain");
    response.setBody("405 - Not Allowed");
  }
}

std::string buildReturnResponse(int code, std::string &url) {
  std::ostringstream oss;
  oss << "HTTP/1.1 " << code << " ";

  switch (code) {
  case 301:
    oss << "Moved Permanently";
    break;
  case 302:
    oss << "Found";
    break;
  case 303:
    oss << "See Other";
    break;
  case 307:
    oss << "Temporary Redirect";
    break;
  case 308:
    oss << "Permanent Redirect";
    break;
  default:
    oss << "Error";
    break;
  }
  std::string body = "<html><body><h1>" + oss.str() + "</h1></body></html>";
  oss << body;
  oss << "Location: " << url << "\r\n";
  return (oss.str());
}

std::string buildErrorResponse(int code) {
  std::ostringstream oss;
  oss << "HTTP/1.1 " << code << " ";

  switch (code) {
  case 400:
    oss << "Bad Request";
    break;
  case 403:
    oss << "Forbidden";
    break;
  case 404:
    oss << "Not Found";
    break;
  case 405:
    oss << "Method not allowed";
    break;
  case 413:
    oss << "Payload Too Large";
    break;
  default:
    oss << "Error";
    break;
  }

  std::string body = "<html><body><h1>" + oss.str() + "</h1></body></html>";
  oss << body;
  return (oss.str());
}

std::string check_error_server(int code, Server &serverConf) {
  std::ostringstream oss;
  oss << code;
  std::string c_error = oss.str();
  std::string err_page = serverConf.get_error_page(c_error);
  if (!err_page.empty())
    return (err_page);
  return ("");
}

std::string set_autoindex(const std::string &filePath) {
  std::ostringstream html;
  struct dirent *entry;
  DIR *dir = opendir(filePath.c_str());

  if (!dir) {
    std::string message =
        std::string("Impossible d'ouvrir le dossier : ") + filePath;
    logError(message);
    return ("");
  }
  html << "<html><head><title>Index of " << filePath << "</title></head><body>";
  html << "<h1>Index of " << filePath << "</h1><ul>";
  while ((entry = readdir(dir)) != NULL) {
    std::string name = entry->d_name;
    if (name == "." || name == "..")
      continue;
    std::string link = filePath;
    if (!link.empty() && link[link.size() - 1] != '/')
      link += '/';
    link += name;
    std::string fullPath = filePath + "/" + name;
    struct stat st;
    if (stat(fullPath.c_str(), &st) == 0 && S_ISDIR(st.st_mode))
      link += '/';
    html << "<li><a href=\"" << link << "\">" << name << "</a></li>";
  }
  closedir(dir);
  html << "</ul></body></html>";
  return (html.str());
}

std::string get_ext(std::string uri) {
  std::size_t found = uri.find_last_of(".");
  if (found != std::string::npos) {
    std::string extension = uri.substr(found);
    return (extension);
  }
  return ("");
}

std::string check_header(std::string uri) {
  std::map<std::string, std::string> c_type;
  c_type[".html"] = "text/html";
  c_type[".js"] = "application/javascript";
  c_type[".css"] = "text/css";
  c_type[".json"] = "application/json";
  c_type[".sh"] = "cgi";
  c_type[".py"] = "cgi";
  std::size_t found = uri.find_last_of(".");

  if (found != std::string::npos) {
    std::string extension = uri.substr(found);
    std::map<std::string, std::string>::iterator it = c_type.find(extension);
    if (it != c_type.end())
      return it->second;
    return ("text/html");
  }
  return ("text/html");
}

int getServerPort(int client_fd) {
  struct sockaddr_in server_address;
  socklen_t server_len = sizeof(server_address);

  if (getsockname(client_fd, (struct sockaddr *)&server_address, &server_len) ==
      -1) {
    perror("getsockname");
    return -1;
  }
  return ntohs(server_address.sin_port);
}

std::string getServerIp(int client_fd) {
  struct sockaddr_in server_address;
  socklen_t server_len = sizeof(server_address);

  if (getsockname(client_fd, (struct sockaddr *)&server_address, &server_len) ==
      -1) {
    perror("getsockname");
    return "";
  }

  char server_ip[INET_ADDRSTRLEN];
  if (inet_ntop(AF_INET, &server_address.sin_addr, server_ip,
                sizeof(server_ip)) == NULL) {
    perror("inet_ntop");
    return "";
  }
  return std::string(server_ip);
}
