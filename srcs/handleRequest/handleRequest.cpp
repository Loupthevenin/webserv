/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:09:00 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/14 15:04:06 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webserv.h"

void handleMethod(int fd, HttpRequest &request, Server &serverConfig)
{
	request.printRequest();

	HttpResponse response;

	// GET POST DELETE A GERER ICI;
	(void)serverConfig;
	if (request.getMethod() == "GET") {
		// handleGet(request, &response, serverConfig);
		response.setStatus(200);
		response.setHeader("Content-Type", "text/html");
		response.setBody("<h1>Bienvenue sur mon server: Hello world!</h1>");
	}
	else if (request.getMethod() == "POST")
	{
		// heandlePost();
	}
	else if (request.getMethod() == "DELETE")
	{
		// handleDelete();
	}
	std::string message = response.toString();
    send(fd, message.c_str(), message.size(), 0);
	close(fd);
}

int readClientData(int fd, Epoll &epoll, std::map<int, HttpRequest> &requests) {
	char buffer[4096];

	int bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read == -1) {
        std::cerr << "Error reading data" << std::endl;
        epoll.removeFd(fd);
        close(fd);
        return -1;
    } else if (bytes_read == 0) {
        std::cout << "Client disconnected" << std::endl;
        epoll.removeFd(fd);
        close(fd);
        return 0;
    }

    buffer[bytes_read] = '\0';

	if (requests.find(fd) == requests.end())
		requests[fd] = HttpRequest();

	requests[fd].setBuffer(buffer, bytes_read);
	return bytes_read;
}

Server *findServerConfig(HttpRequest &request, ConfigParser &conf)
{
	std::string client_host = request.getHost();

	for (size_t i = 0; i < conf.servers.size(); i++)
	{
		std::string server_host = conf.servers[i].get_mapValue("server_name").getString();
		if (server_host == client_host)
			return &conf.servers[i];
	}
	return NULL;
}

void handleRequest(int fd, Epoll &epoll, ConfigParser &conf) {
	static std::map<int, HttpRequest> requests;

	int bytes_read = readClientData(fd, epoll, requests);
	if (bytes_read <= 0) return;

	HttpRequest &request = requests[fd];

	if (!request.isHeaderComplete())
		request.appendRawData(std::string(request.getBuffer(), bytes_read));
	else if (request.getMethod() == "POST")
	{
		if (request.getContentLength() > 0)
		{
			request.appendBodyData(std::string(request.getBuffer(), bytes_read));
			Server *serverConfig = findServerConfig(request, conf);
				if (serverConfig)
				{
					// size_t maxBodySize = serverConfig->get_mapValue("client_max_body_size").getInt();
					size_t maxBodySize = 1024*1024;
					// verif le bodySize;
					if (request.getBody().size() > maxBodySize) {
						std::cerr << "Body size exceeds maximum allowed limits!" << std::endl;
						sendError(fd, 413, "<h1>413 Request Entity Too Large</h1>");
						closeConnexion(fd, epoll, requests);
						return;
				}
			}
		}
	}

	if (static_cast<size_t>(request.getBody().size()) >= static_cast<size_t>(request.getContentLength()))
	{
		Server *serverConfig = findServerConfig(request, conf);
		if (!serverConfig)
		{
			std::cerr << "No matching server block found for host" << std::endl;
			sendError(fd, 404, "<h1>404 Not Found</h1>");
			closeConnexion(fd, epoll, requests);
			return;
		}
		handleMethod(fd, request, *serverConfig);
		requests.erase(fd);
	}
}
