/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIExec.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 09:43:53 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/23 13:28:58 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/CGIExec.hpp"
#include <sys/wait.h>
#include <unistd.h>

CGIExec::CGIExec() : scriptPath(""), cgi_fd(-1), cgi_pid(0) {}

CGIExec::CGIExec(const std::string &script, const HttpRequest &request,
                 int client_fd)
    : scriptPath(script), cgi_fd(client_fd), cgi_pid(0) {
  if (pipe(pipe_in) == -1) {
    std::cerr << "pipe: Error" << std::endl;
    return;
  }
  setupEnvironment(request);
}

CGIExec::~CGIExec() {}

void CGIExec::setupEnvironment(const HttpRequest &request) {
  env["REQUEST_METHOD"] = request.getMethod();
  env["REQUEST_URI"] = request.getUri();
  env["CONTENT_LENGTH"] = request.getHeader("Content-Length");
  env["CONTENT_TYPE"] = request.getHeader("Content-Type");
  env["SCRIPT_FILENAME"] = scriptPath;
  env["GATEWAY_INTERFACE"] = "CGI/1.1";
  env["SERVER_PROTOCOL"] = "HTTP/1.1";
  env["REDIRECT_STATUS"] = "200";
  // env["QUERYY_STRING"] = request.getQuery();
}

bool CGIExec::isValidScriptPath() const {
  if (access(scriptPath.c_str(), F_OK) == -1) {
    std::cerr << "Error: Script not found: " << scriptPath << std::endl;
    return false;
  }
  if (access(scriptPath.c_str(), X_OK) == -1) {
    std::cerr << "Error: Script is not executable: " << scriptPath << std::endl;
    return false;
  }
  return true;
}

char **CGIExec::convertEnvToCharArray() {
  char **result = new char *[env.size() + 1];
  int i = 0;

  for (std::map<std::string, std::string>::const_iterator it = env.begin();
       it != env.end(); ++it) {
    std::string entry = it->first + "=" + it->second;
    result[i] = new char[entry.size() + 1];
    std::cout << entry << std::endl;
    std::strcpy(result[i], entry.c_str());
    i++;
  }
  result[i] = NULL;

  return result;
}

char **CGIExec::convertArgsToCharArray(const std::vector<std::string> &args) {
  char **result = new char *[args.size() + 1];

  for (size_t i = 0; i < args.size(); ++i) {
    result[i] = new char[args[i].size() + 1];
    std::strcpy(result[i], args[i].c_str());
  }
  result[args.size()] = NULL;

  return result;
}

void CGIExec::freeCharArray(char **array) const {
  if (array == NULL)
    return;
  int i = 0;
  while (array[i]) {
    delete[] array[i];
    i++;
  }
  delete[] array;
}

int CGIExec::getPipeIn() const { return pipe_in[0]; }
int CGIExec::getPipeOut() const { return pipe_out[1]; }
pid_t CGIExec::getPid() const { return cgi_pid; }
int CGIExec::getClientFd() const { return cgi_fd; }

int CGIExec::execute() {

  if (!isValidScriptPath()) {
    return -1;
  }

  pid_t pid = fork();
  if (pid < 0) {
    perror("fork");
    return -1;
  }
  if (pid == 0) {
    close(pipe_in[1]);

    dup2(pipe_in[0], STDIN_FILENO);
    dup2(cgi_fd, STDOUT_FILENO);

    close(pipe_in[0]);

    std::vector<std::string> args;
    args.push_back(scriptPath);

    char **envp = convertEnvToCharArray();
    char **cargs = convertArgsToCharArray(args);

    execve(cargs[0], cargs, envp);
    perror("execve");
    freeCharArray(cargs);
    freeCharArray(envp);
    exit(EXIT_FAILURE);
  }
  cgi_pid = pid;
  close(pipe_in[0]);
  close(pipe_in[1]);
  // close(cgi_fd);
  return 0;
}
