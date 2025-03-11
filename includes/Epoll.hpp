/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoll.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 16:21:59 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/10 16:43:24 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>

class Epoll {
private:
  int epoll_fd;
  int max_events;

public:
  Epoll(int max_events);
  ~Epoll();

  void addFd(int fd, int events);
  void removeFd(int fd);
  int waitForEvents(std::vector<struct epoll_event> &events);
};
