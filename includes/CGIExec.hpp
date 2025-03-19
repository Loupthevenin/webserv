/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIExec.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 09:44:07 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/19 15:46:28 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "HttpRequest.hpp"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

class CGIExec {
private:
  std::string scriptPath;
  std::map<std::string, std::string> env;
  int pipe_in[2];
  int pipe_out[2];
  int cgi_fd;
  bool isComplete;
  pid_t cgi_pid;
  std::string cgiOutput;

  std::string getInterpreter(const std::string &extension) const;

public:
  CGIExec();
  CGIExec(const std::string &script, const HttpRequest &request, int client_fd);
  ~CGIExec();

  int getPipeIn() const;
  int getPipeOut() const;
  bool getIsComplete() const;
  pid_t getPid() const;
  std::string getCGIOutput() const;
  int getClientFd() const;

  int execute();
  int readCGIOutput();
};
