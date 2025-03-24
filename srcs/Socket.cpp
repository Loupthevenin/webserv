/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 12:25:20 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/24 15:44:18 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Socket.hpp"
#include <cstdlib>
#include <netinet/in.h>
#include <sys/socket.h>

Socket::Socket() {
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd == -1) {
    perror("Failed to create socket");
    exit(EXIT_FAILURE);
  }
}

Socket::Socket(const Socket &other) {
	(void)other;
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd == -1) {
    perror("Failed to create socket");
    exit(EXIT_FAILURE);
  }
}

Socket::~Socket() { close(socket_fd); }

void Socket::setSocketNonBlocking(int fd) {
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

void Socket::bindToAddress(int port) {
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(port);
  int opt = 1;

  setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  if (bind(socket_fd, (struct sockaddr *)&server_address,
           sizeof(server_address)) < 0) {
    perror("Bind failed");
    exit(EXIT_FAILURE);
  }
}

void Socket::listenForConnections(int backlog) {
  if (listen(socket_fd, backlog) < 0) {
    perror("Listen failed");
    exit(EXIT_FAILURE);
  }
}

int Socket::acceptConnection() {
  sockaddr client_address;
  socklen_t client_len = sizeof(client_address);
  int client_fd =
      accept(socket_fd, (struct sockaddr *)&client_address, &client_len);
  if (client_fd == -1)
    perror("Accept failed");
  return client_fd;
}

int Socket::getFd() const { return socket_fd; }

std::string Socket::getClientIp(int client_fd) {
	struct sockaddr_in client_address;
	socklen_t client_len = sizeof(client_address);

	if (getpeername(client_fd, (struct sockaddr *)&client_address, &client_len) == -1) {
		perror("getpeername");
		return "";
	}

	char client_ip[INET_ADDRSTRLEN];
	if (inet_ntop(AF_INET, &client_address.sin_addr, client_ip, sizeof(client_ip)) == NULL)
	{
		perror("inet_ntop");
		return "";
	}

	return std::string(client_ip);
}

int Socket::getClientPort(int client_fd) {
	struct sockaddr_in client_address;
	socklen_t client_len = sizeof(client_address);
	if (getpeername(client_fd, (struct sockaddr *)&client_address, &client_len) == -1) {
        perror("getpeername");
		return -1;
    }

    return ntohs(client_address.sin_port);
}
