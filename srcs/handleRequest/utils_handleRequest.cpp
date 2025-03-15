/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_handleRequest.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 14:57:19 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/15 11:22:17 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webserv.h"

void sendError(int fd, int statusCode, const std::string &body)
{
	HttpResponse response;
	response.setStatus(statusCode);
	// filepath here;
	response.setBody(body);

	std::string message = response.toString();
	send(fd, message.c_str(), message.size(), 0);
}

void closeConnexion(int fd, Epoll &epoll, std::map<int, HttpRequest> &requests)
{
	requests.erase(fd);
	epoll.removeFd(fd);
	close(fd);
}

std::string readFile(const std::string &filePath)
{
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
