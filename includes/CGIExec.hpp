/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIExec.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 09:44:07 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/24 14:37:03 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "HttpRequest.hpp"
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
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
  int cgi_fd;
  pid_t cgi_pid;
  int httpErrorCode;

  void setupEnvironment(const HttpRequest &request);
  bool isValidScriptPath();
  char **convertEnvToCharArray();
  char **convertArgsToCharArray(const std::vector<std::string> &args);
  void freeCharArray(char **array) const;

public:
  CGIExec();
  CGIExec(const std::string &script, const HttpRequest &request, int client_fd);
  ~CGIExec();

  int getPipeIn() const;
  pid_t getPid() const;
  int getClientFd() const;
  int getHttpErrorCode() const;

  int execute(std::string body);
};
