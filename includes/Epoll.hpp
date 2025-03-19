/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoll.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 16:21:59 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/19 19:18:32 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "CGIExec.hpp"
#include "HttpResponse.hpp"
#include "../includes/signal.hpp"
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>

class Epoll {
private:
  int epoll_fd;
  int max_events;
  std::map<int, CGIExec> cgiExecs;

public:
  Epoll(int max_events);
  ~Epoll();

  std::map<int, CGIExec> getCGIExecs() const;

  void addFd(int fd, int events);
  void removeFd(int fd);
  int waitForEvents(std::vector<struct epoll_event> &events);

  void addCGIProcess(int fd, CGIExec &cgi);
  void isCGIFdInEvents(std::vector<struct epoll_event> &events);
  void handleCGIOutput(int fd);

	void clean();
};
