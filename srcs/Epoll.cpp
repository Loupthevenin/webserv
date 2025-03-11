/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoll.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 16:21:46 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/10 16:43:51 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Epoll.hpp"

Epoll::Epoll(int max_events) : max_events(max_events) {
  epoll_fd = epoll_create1(0);
  if (epoll_fd == -1) {
    perror("Failed to create epoll instance");
    exit(EXIT_FAILURE);
  }
}

Epoll::~Epoll() { close(epoll_fd); }

void Epoll::addFd(int fd, int events) {
  struct epoll_event event;

  event.events = events;
  event.data.fd = fd;

  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1) {
    perror("Failed to add fd to epoll");
    exit(EXIT_FAILURE);
  }
}

void Epoll::removeFd(int fd) {
  if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL) == -1) {
    perror("Failed to remove fd from epoll");
    exit(EXIT_FAILURE);
  }
}

int Epoll::waitForEvents(std::vector<struct epoll_event> &events) {
  int event_count = epoll_wait(epoll_fd, events.data(), max_events, -1);
  if (event_count == -1) {
    perror("Failed to wait for events");
    exit(EXIT_FAILURE);
  }
  return event_count;
}
