/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:09:00 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/13 10:33:44 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webserv.h"

void handleMethod(int fd, HttpRequest &request)
{
	request.printRequest();

	HttpResponse response;

	// GET POST DELETE A GERER ICI;
	if (request.getMethod() == "GET") {
		// handleGet();
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

void handleRequest(int fd, Epoll &epoll) {
	char buffer[4096];
	static std::map<int, HttpRequest> requests;

	int bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read == -1) {
        std::cerr << "Error reading data" << std::endl;
        epoll.removeFd(fd);
        close(fd);
        return;
    } else if (bytes_read == 0) {
        std::cout << "Client disconnected" << std::endl;
        epoll.removeFd(fd);
        close(fd);
        return;
    }

    buffer[bytes_read] = '\0';
	std::cout << "nb read: " << bytes_read << std::endl;

	if (requests.find(fd) == requests.end())
		requests[fd] = HttpRequest();

	HttpRequest &request = requests[fd];

	if (!request.isHeaderComplete())
		request.appendRawData(std::string(buffer, bytes_read));
	else if (request.getMethod() == "POST")
	{
		if (request.getContentLength() > 0)
			request.appendBodyData(std::string(buffer, bytes_read));
	}
	if (static_cast<size_t>(request.getBody().size()) >= static_cast<size_t>(request.getContentLength()))
	{
		handleMethod(fd, request);
		requests.erase(fd);
	}
}
