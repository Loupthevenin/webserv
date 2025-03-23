/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdibia <opdibia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 11:08:41 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/23 15:13:52 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.h"

static void initializeServer(Socket &serverSocket, Epoll &epoll, int port) {
  serverSocket.bindToAddress(port);
  std::cout << "Bind port: " << port << std::endl;
  serverSocket.setSocketNonBlocking(serverSocket.getFd());
  serverSocket.listenForConnections();
  std::cout << "Waiting for connexions..." << std::endl;
  epoll.addFd(serverSocket.getFd(), EPOLLIN);
}

static void handleNewConnection(Socket &serverSocket, Epoll &epoll) {

  int client_fd = serverSocket.acceptConnection();
  std::cout << "Connexion accept !" << std::endl;
  serverSocket.setSocketNonBlocking(client_fd);
  epoll.addFd(client_fd, EPOLLIN);
}

static void eventLoop(Epoll &epoll, std::vector<Socket> &serverSockets,
               ConfigParser &conf) {
  std::vector<struct epoll_event> events(10);

  while (g_running) {
    int event_count = epoll.waitForEvents(events);
    if (!g_running)
      break;
    for (int i = 0; i < event_count; i++) {
      int fd = events[i].data.fd;
      bool isServerSocket = false;
      for (size_t j = 0; j < serverSockets.size(); j++) {
        if (fd == serverSockets[j].getFd()) {
          handleNewConnection(serverSockets[j], epoll);
          isServerSocket = true;
          break;
        }
      }
      if (!isServerSocket && (events[i].events & EPOLLIN)) {
        handleRequest(fd, epoll, conf);
      }
    }
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Error ./webserv *.conf" << std::endl;
    return (-1);
  }
  signal(SIGINT, signalHandler);
  try {
    ConfigParser conf(argv[1]);
    conf.parseConfig();

    Epoll epoll(10);
    std::vector<Socket> serverSockets(conf.servers.size());
    std::set<int> ports;

    for (size_t i = 0; i < conf.servers.size(); i++) {
      int port = conf.servers[i].get_port();
      if (ports.find(port) == ports.end()) {
        initializeServer(serverSockets[i], epoll, port);
        ports.insert(port);
      }
      // conf.display_config();
    }
    eventLoop(epoll, serverSockets, conf);
  } catch (std::exception &e) {
    std::cerr << e.what() << '\n';
    return (-1);
  }
  return EXIT_SUCCESS;
}
