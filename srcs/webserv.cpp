/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 11:08:41 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/11 12:16:45 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.h"

void initializeServer(Socket &serverSocket, Epoll &epoll, int port) {
  serverSocket.bindToAddress(port);
  std::cout << "Bind port: " << port << std::endl;
  serverSocket.setSocketNonBlocking(serverSocket.getFd());
  serverSocket.listenForConnections();
  std::cout << "Waiting for connexions..." << std::endl;
  epoll.addFd(serverSocket.getFd(), EPOLLIN);
}

void handleNewConnection(Socket &serverSocket, Epoll &epoll) {

  int client_fd = serverSocket.acceptConnection();
  std::cout << "Connexion accept !" << std::endl;
  serverSocket.setSocketNonBlocking(client_fd);
  epoll.addFd(client_fd, EPOLLIN);
}

void handleRequest(int fd, Epoll &epoll) {
  char buffer[1024];
  int bytes_read = read(fd, buffer, sizeof(buffer) - 1);

  if (bytes_read == -1) {
    perror("Read failed");
    epoll.removeFd(fd);
    close(fd);
  } else if (bytes_read == 0) {
    std::cout << "Client disconnected" << std::endl;
    epoll.removeFd(fd);
    close(fd);
  } else {
    buffer[bytes_read] = '\0';
    std::cout << "Request receive: " << buffer << std::endl;

    std::string message = "HTTP/1.1 200 OK\r\n"
                          "Content-Type: text/plain\r\n"
                          "Content-Length: 13\r\n"
                          "\r\n"
                          "Hello, world!\n";
    send(fd, message.c_str(), message.size(), 0);
    epoll.removeFd(fd);
    close(fd);
    std::cout << "Response, connexion close." << std::endl;
  }
}

void eventLoop(Epoll &epoll, Socket &serverSocket) {
  std::vector<struct epoll_event> events(10);

  while (true) {
    int event_count = epoll.waitForEvents(events);
    for (int i = 0; i < event_count; i++) {
      int fd = events[i].data.fd;

      if (fd == serverSocket.getFd()) {
        handleNewConnection(serverSocket, epoll);
      } else if (events[i].events & EPOLLIN) {
        handleRequest(fd, epoll);
      }
    }
  }
}

int main(int argc, char **argv) {
  (void)argv;
  (void)argc;

  int port = 8080;
  Socket serverSocket;
  Epoll epoll(10);

  initializeServer(serverSocket, epoll, port);
  eventLoop(epoll, serverSocket);

  return EXIT_SUCCESS;
}
