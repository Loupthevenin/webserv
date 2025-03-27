/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdi-bia <opdi-bia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:09:00 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/27 15:15:25 by opdi-bia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webserv.h"

void handleMethod(int fd, HttpRequest &request, Server &serverConfig) {
  request.printRequest();

  HttpResponse response;

  if (request.getMethod() == "GET") {
    logInfo("HTTP", "Request GET", _BLUE);
    if ("/methods/files" == request.getUri()) {
      std::string body = listFilesInDirectory("www/methods/delete");
      response.setStatus(200);
      response.setHeader("Content-Type", "application/json");
      response.setBody(body);
    } else if (handleGet(request, response, serverConfig, fd) == 1)
      return;
  } else if (request.getMethod() == "POST") {
    if(handlePost(request, response, serverConfig, fd) == 1)
      return;
    logInfo("HTTP", "Request POST", _PURPLE);
  } else if (request.getMethod() == "DELETE") {
    handleDelete(request, response, serverConfig, fd);
    logInfo("HTTP", "Request DELETE", _CYAN);
  }
  // std::cout << "status = " << response.getStatus()
  //           << " body = " << response.getBody() << std::endl;
  std::string message = response.toString();
  send(fd, message.c_str(), message.size(), 0);
  close(fd);
}

int readClientData(int fd, Epoll &epoll, std::map<int, HttpRequest> &requests,
                   std::string &buffer) {

  int bytes_read = read(fd, &buffer[0], buffer.size() - 1);
  if (bytes_read == -1) {
    logError("reading data");
    epoll.removeFd(fd);
    close(fd);
    return -1;
  } else if (bytes_read == 0) {
    std::cout << "Client disconnected" << std::endl;
    epoll.removeFd(fd);
    close(fd);
    return 0;
  }
  buffer.resize(bytes_read);
  if (requests.find(fd) == requests.end())
    requests[fd] = HttpRequest();
  return bytes_read;
}

Server *findServerConfig(HttpRequest &request, ConfigParser &conf) {
  std::string client_host_raw = request.getHost();
  std::string client_host;
  int client_port = 8080;

  size_t colon = client_host_raw.find(':');
  if (colon != std::string::npos) {
    client_host = client_host_raw.substr(0, colon);
    std::string port_str = client_host_raw.substr(colon + 1);
    std::istringstream iss(port_str);
    iss >> client_port;
    if (iss.fail()) {
      std::string message =
          std::string("Invalid port in Host header: ") + port_str;
      logError(message);
      return (NULL);
    }
  } else {
    client_host = client_host_raw;
  }
  Server *default_server = NULL;

  for (size_t i = 0; i < conf.servers.size(); i++) {
    Server &srv = conf.servers[i];

    std::string server_host = srv.get_server_name();
    int server_port = srv.get_port();
    if (server_port == client_port) {
      if (server_host == client_host)
        return &conf.servers[i];

      if (default_server == NULL)
        default_server = &conf.servers[i];
    }
  }
  return (default_server);
}

void handleRequest(int fd, Epoll &epoll, ConfigParser &conf) {
  static std::map<int, HttpRequest> requests;
  std::string buffer(4096, '\0');

  int bytes_read = readClientData(fd, epoll, requests, buffer);
  if (bytes_read <= 0)
    return;

  HttpRequest &request = requests[fd];

  if (!request.isHeaderComplete())
    request.appendRawData(buffer);
  else if (request.getMethod() == "POST") {
    if (request.getContentLength() > 0) {
      request.appendBodyData(buffer);
      Server *serverConfig = findServerConfig(request, conf);
      if (serverConfig) {
        size_t maxBodySize = serverConfig->get_body_client();
        if (request.getBody().size() > maxBodySize) {
          logError("Body size exceeds maximum allowed limits!");
          sendError(fd, 413, "<h1>413 Request Entity Too Large</h1>");
          closeConnexion(fd, epoll, requests);
          return;
        }
      }
    }
  }

  if (static_cast<size_t>(request.getBody().size()) >=
      static_cast<size_t>(request.getContentLength())) {
    Server *serverConfig = findServerConfig(request, conf);
    if (!serverConfig) {
      logError("No matching server block found for host");
      sendError(fd, 404, "<h1>404 Not Found</h1>");
      closeConnexion(fd, epoll, requests);
      return;
    }
    handleMethod(fd, request, *serverConfig);
    requests.erase(fd);
  }
}
