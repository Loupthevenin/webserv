/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Epoll.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 16:21:46 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/19 19:20:10 by ltheveni         ###   ########.fr       */
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

std::map<int, CGIExec> Epoll::getCGIExecs() const { return cgiExecs; }

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
	// on passe dans perror + exit quand SIGINT;
	// if (event_count == -1) {
	// 	perror("Failed to wait for events");
	// 	exit(EXIT_FAILURE);
	// }
	return event_count;
}

void Epoll::addCGIProcess(int fd, CGIExec &cgi) { cgiExecs[fd] = cgi; }

void Epoll::isCGIFdInEvents(std::vector<struct epoll_event> &events) {
  std::map<int, CGIExec>::iterator it = cgiExecs.begin();

  while (it != cgiExecs.end()) {
    int cgi_fd = it->first;
    bool found = false;
    for (size_t i = 0; i < events.size(); ++i) {
      if (events[i].data.fd == cgi_fd) {
        found = true;
        break;
      }
    }
    if (!found) {
      if (cgi_fd != -1)
        addFd(cgi_fd, EPOLLIN);
    }
    ++it;
  }
}

void Epoll::handleCGIOutput(int pipe_fd) {
  std::map<int, CGIExec>::iterator it = cgiExecs.find(pipe_fd);
  if (it != cgiExecs.end()) {
    // a revoir pour les errors;
    if (it->second.readCGIOutput() == 0 || it->second.getIsComplete()) {
      HttpResponse response;

      response.setStatus(200);
      response.setHeader("Content-Type", "text/html");
      response.setBody(it->second.getCGIOutput());

      std::string message = response.toString();
      int client_fd = it->second.getClientFd();
      send(client_fd, message.c_str(), message.size(), 0);

      removeFd(pipe_fd);
      close(pipe_fd);
      cgiExecs.erase(it);
      close(client_fd);
      pid_t pid = it->second.getPid();
      if (pid > 0) {
        int status;
        waitpid(pid, &status, WNOHANG);
      }
    }
  }
}

void Epoll::clean() {
	for (std::map<int, CGIExec>::iterator it = cgiExecs.begin(); it != cgiExecs.end(); ++it)
	{
		// close client_fd CGI;
		close(it->second.getClientFd());
		// close pipe_out[0] -> STDOUT;
		close(it->first);
	}
	cgiExecs.clear();
}
