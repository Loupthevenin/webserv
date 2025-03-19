/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_handleRequest.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 14:57:19 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/18 20:42:41 by ltheveni         ###   ########.fr       */
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

std::string listFilesInDirectory(const std::string &directory)
{
	std::ostringstream oss;
	oss << "{ \"files\": [";

	DIR *dir = opendir(directory.c_str());
	if (!dir)
		return "{ \"Error\": \"open directory\"";

	struct dirent *entry;
	bool first = true;
	while ((entry = readdir(dir)) != NULL) {
		std::string filename(entry->d_name);
		if (filename != "." && filename != "..") {
			if (!first) oss << ", ";
			first = false;
			oss << "\"" << filename << "\"";
		}
	}

	closedir(dir);
	oss << "] }";
	return oss.str();
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
