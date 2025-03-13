/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdibia <opdibia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 11:08:41 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/13 16:37:28 by opdibia          ###   ########.fr       */
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

// int main(int argc, char **argv) {
//   (void)argv;
//   (void)argc;

//   int port = 8080;
//   Socket serverSocket;
//   Epoll epoll(10);

//   initializeServer(serverSocket, epoll, port);
//   eventLoop(epoll, serverSocket);

//   return EXIT_SUCCESS;
// }

int main(int argc, char **argv) {
  Socket serverSocket;
  Epoll epoll(10);
  
  if (argc != 2){
    std::cerr << "Error ./webserv *.conf" << std::endl;
    return(-1);
  }
  try{
    ConfigParser conf(argv[1]);
    conf.parseConfig();
    // conf.display_config();
    int port = conf.servers[0].get_mapValue("port").getInt();
    initializeServer(serverSocket, epoll, port);
    eventLoop(epoll, serverSocket);
  }
  catch(std::exception & e){
    std::cerr << e.what() << '\n';
    return(-1);
  }
  return EXIT_SUCCESS;
}