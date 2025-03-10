/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 12:25:33 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/10 14:03:48 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

class Socket {
private:
  int socket_fd;

public:
  Socket();
  ~Socket();

  void setSocketNonBlocking(int fd);
  void bindToAddress(int port);
  void listenForConnections(int backlog = SOMAXCONN);
  int acceptConnection();

  int getFd() const;
};
