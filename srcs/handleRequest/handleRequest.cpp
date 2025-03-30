/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdi-bia <opdi-bia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:09:00 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/30 13:31:19 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webserv.h"

static void handleMethod(int fd, HttpRequest &request, Server &serverConfig) {
  HttpResponse response;

  if (request.getMethod() == "GET") {
    logInfo("HTTP", "Request GET", request.getUri(), _BLUE);
    if ("/methods/files" == request.getUri()) {
      std::string body = listFilesInDirectory("www/methods/delete");
      response.setStatus(200);
      response.setHeader("Content-Type", "application/json");
      response.setBody(body);
    } else if (handleGet(request, response, serverConfig, fd) == 1)
      return;
  } else if (request.getMethod() == "POST") {
    if (handlePost(request, response, serverConfig, fd) == 1)
      return;
    logInfo("HTTP", "Request POST", request.getUri(), _PURPLE);
  } else if (request.getMethod() == "DELETE") {
    handleDelete(request, response, serverConfig, fd);
    logInfo("HTTP", "Request DELETE", request.getUri(), _CYAN);
  }
  // std::cout << "status = " << response.getStatus()
  //           << " body = " << response.getBody() << std::endl;
  std::string message = response.toString();
  send(fd, message.c_str(), message.size(), 0);
  close(fd);
}

static int readClientData(int fd, Epoll &epoll, std::map<int, HttpRequest> &requests,
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

static Server *findServerConfig(HttpRequest &request, ConfigParser &conf, int fd) {
  std::string client_host_raw = request.getHost();
  std::string client_host;
	int client_host_port;
  int client_port = getServerPort(fd);
	std::string client_ip = getServerIp(fd);

  size_t colon = client_host_raw.find(':');
  if (colon != std::string::npos) {
    client_host = client_host_raw.substr(0, colon);
    std::string port_str = client_host_raw.substr(colon + 1);
    std::istringstream iss(port_str);
    iss >> client_host_port;
    if (iss.fail()) {
      std::string message =
          std::string("Invalid port in Host header: ") + port_str;
      logError(message);
      return (NULL);
    }
  } else {
    client_host = client_host_raw;
  }

	std::cout << "client_ip: " << client_ip << ", client_port: " << client_port << ", client_host: " << client_host << std::endl;
	std::vector<Server *> vec_servers;

  for (size_t i = 0; i < conf.servers.size(); i++) {
    Server &srv = conf.servers[i];

    std::string server_conf_host = srv.get_server_name();
	std::string server_conf_ip = srv.get_host();
    int server_conf_port = srv.get_port();
		std::cout << "server_ip: " << server_conf_ip << ", server_port: " << server_conf_port << ", server_host: " << server_conf_host << std::endl;

		if (server_conf_port == client_port) {
			if (server_conf_ip == client_ip || server_conf_ip == "0.0.0.0")
				vec_servers.push_back(&conf.servers[i]);
			}
	}

	if (vec_servers.size() > 1) {
		for (size_t i = 0; i < vec_servers.size(); ++i) {
			std::string server_conf_host = vec_servers[i]->get_server_name();
			if (server_conf_host.empty() || server_conf_host == client_host)
				return vec_servers[i];
		}
	}
	else if (vec_servers.size() == 1) {
		return vec_servers[0];
	}
	logError("404 Not Found: No matching server_name block found for host");
  return (NULL);
}

void handleRequest(int fd, Epoll &epoll, ConfigParser &conf) {
  static std::map<int, HttpRequest> requests;
  std::string buffer(4096, '\0');

  int bytes_read = readClientData(fd, epoll, requests, buffer);
  if (bytes_read <= 0)
    return;

  HttpRequest &request = requests[fd];

  if (!request.isHeaderComplete()) {
    try {
      request.appendRawData(buffer);
    } catch (const std::exception &e) {
      logError(e.what());
      // 400 BAD REQUEST
      sendError(fd, 400, "");
      closeConnexion(fd, epoll, requests);
      return;
    }
  } else if (request.getMethod() == "POST" || request.getMethod() == "DELETE") {
    if (request.getContentLength() > 0)
      request.appendBodyData(buffer);
	}

  if (static_cast<size_t>(request.getBody().size()) >=
      static_cast<size_t>(request.getContentLength())) {
    Server *serverConfig = findServerConfig(request, conf, fd);
    if (!serverConfig) {
      sendError(fd, 404, "<h1>404 Not Found</h1>");
      closeConnexion(fd, epoll, requests);
      return;
    }
    handleMethod(fd, request, *serverConfig);
    requests.erase(fd);
  }
}
