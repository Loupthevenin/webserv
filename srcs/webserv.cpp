/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 11:08:41 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/10 14:52:15 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Socket.hpp"
#include <cstdlib>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, char **argv) {
  (void)argv;
  (void)argc;
  Socket serverSocket;
  int port = 8080;

  serverSocket.bindToAddress(port);
  std::cout << "Bind port: " << port << std::endl;

  serverSocket.listenForConnections();
  std::cout << "Waiting for connexions..." << std::endl;

  while (true) {
    int client_fd = serverSocket.acceptConnection();
    if (client_fd != -1) {
      std::cout << "Connexion accept !" << std::endl;

      // recupere l'address du client;
      struct sockaddr_in client_addr;
      socklen_t client_addr_len = sizeof(client_addr);
      getpeername(client_fd, (struct sockaddr *)&client_addr, &client_addr_len);
      std::cout << "Client connect: " << inet_ntoa(client_addr.sin_addr) << ":"
                << ntohs(client_addr.sin_port) << std::endl;

      // envoi de la reponse au client la reponse doit respecter le format http
      // sinon ca failed
      std::string message = "HTTP/1.1 200 OK\r\n"
                            "Content-Type: text/plain\r\n"
                            "Content-Length: 13\r\n"
                            "\r\n"
                            "Hello, world!\n";
      send(client_fd, message.c_str(), message.size(), 0);
      close(client_fd);
      std::cout << "Response, connexion close." << std::endl;
    }
  }
  return EXIT_SUCCESS;
}
