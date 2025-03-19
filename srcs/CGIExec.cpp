/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIExec.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 09:43:53 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/19 19:13:40 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/CGIExec.hpp"

CGIExec::CGIExec()
    : scriptPath(""), cgi_fd(-1), isComplete(false), cgi_pid(0) {}

CGIExec::CGIExec(const std::string &script, const HttpRequest &request,
                 int client_fd)
    : scriptPath(script), cgi_fd(client_fd), isComplete(false), cgi_pid(0) {
  if (pipe(pipe_out) == -1 || pipe(pipe_in) == -1) {
    std::cerr << "pipe: Error" << std::endl;
    return;
  }
  env["REQUEST_METHOD"] = request.getMethod();
  env["REQUEST_URI"] = request.getUri();
  env["CONTENT-LENGTH"] = request.getHeader("Content-Length");
  env["CONTENT-TYPE"] = request.getHeader("Content-Type");
}

CGIExec::~CGIExec() {
// pas clair ici
  close(pipe_in[0]);
  close(pipe_in[1]);
  close(pipe_out[1]);
}

int CGIExec::getPipeIn() const { return pipe_in[0]; }

int CGIExec::getPipeOut() const { return pipe_out[1]; }

bool CGIExec::getIsComplete() const { return isComplete; }

pid_t CGIExec::getPid() const { return cgi_pid; }

std::string CGIExec::getCGIOutput() const { return cgiOutput; }

int CGIExec::getClientFd() const { return cgi_fd; }

std::string CGIExec::getInterpreter(const std::string &extension) const {
  if (extension == ".py")
    return "/usr/bin/python3";
  else if (extension == ".sh")
    return "/usr/bin/bash";
  return NULL;
}

int CGIExec::execute() {
  pid_t pid = fork();
  if (pid < 0) {
    perror("fork");
    return -1;
  }
  if (pid == 0) {
    close(pipe_out[0]);
    close(pipe_in[1]);

    dup2(pipe_in[0], STDIN_FILENO);
    dup2(pipe_out[1], STDOUT_FILENO);

    close(pipe_in[0]);
    close(pipe_out[1]);

    std::vector<char *> envp;
    for (std::map<std::string, std::string>::const_iterator it = env.begin();
         it != env.end(); ++it) {
      std::string envVar = it->first + "=" + it->second;
      envp.push_back(const_cast<char *>(envVar.c_str()));
    }
    envp.push_back(NULL);

    std::vector<std::string> args;
    std::string extension = scriptPath.substr(scriptPath.rfind('.'));
    std::string interpreter = getInterpreter(extension);
    args.push_back(interpreter);
    args.push_back(scriptPath);

    std::vector<char *> cargs;
    for (size_t i = 0; i < args.size(); ++i)
      cargs.push_back(const_cast<char *>(args[i].c_str()));
    cargs.push_back(NULL);

    // check access
    if (access(cargs[1], X_OK) == -1) {
      perror("Script not executable");
      exit(EXIT_FAILURE);
    }

    execve(cargs[0], cargs.data(), envp.data());
    perror("execve:");
    exit(EXIT_FAILURE);
  }
  cgi_pid = pid;
  close(pipe_in[0]);
  close(pipe_out[1]);

  return pipe_out[0];
}

int CGIExec::readCGIOutput() {
  char buffer[4096];
  int bytes_read = read(pipe_out[0], buffer, sizeof(buffer));
  if (bytes_read == -1) {
    std::cerr << "Error reading data" << std::endl;
    return -1;
  }
  if (bytes_read == 0) {
    isComplete = true;
    return -1;
  }
  cgiOutput.append(buffer, bytes_read);

  return 0;
}
