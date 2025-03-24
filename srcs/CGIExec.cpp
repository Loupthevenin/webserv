/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIExec.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 09:43:53 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/24 14:42:23 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/CGIExec.hpp"

CGIExec::CGIExec() : scriptPath(""), cgi_fd(-1), cgi_pid(0), httpErrorCode(0) {}

CGIExec::CGIExec(const std::string &script, const HttpRequest &request,
                 int client_fd)
    : scriptPath(script), cgi_fd(client_fd), cgi_pid(0), httpErrorCode(0) {
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
  env["CONTENT_LENGTH"] = request.getHeader("content-length");
  env["CONTENT_TYPE"] = request.getHeader("content-type");
  env["SCRIPT_FILENAME"] = scriptPath;
  env["GATEWAY_INTERFACE"] = "CGI/1.1";
  env["SERVER_PROTOCOL"] = "HTTP/1.1";
  env["REDIRECT_STATUS"] = "200";
  env["QUERY_STRING"] = request.getQuery();
  env["HTTP_COOKIE"] = request.getHeader("cookie");
}

bool CGIExec::isValidScriptPath() {
  if (access(scriptPath.c_str(), F_OK) == -1) {
    std::cerr << "Error: Script not found: " << scriptPath << std::endl;
    httpErrorCode = 404;
    return false;
  }
  if (access(scriptPath.c_str(), X_OK) == -1) {
    std::cerr << "Error: Script is not executable: " << scriptPath << std::endl;
    httpErrorCode = 403;
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
pid_t CGIExec::getPid() const { return cgi_pid; }
int CGIExec::getClientFd() const { return cgi_fd; }
int CGIExec::getHttpErrorCode() const { return httpErrorCode; }

int CGIExec::execute(std::string body) {

  if (!isValidScriptPath()) {
    close(pipe_in[0]);
    close(pipe_in[1]);
    return -1;
  }

  pid_t pid = fork();
  if (pid < 0) {
    perror("fork");
    httpErrorCode = 500;
    close(pipe_in[0]);
    close(pipe_in[1]);
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
  if (!body.empty()) {
    ssize_t bytes_written = write(pipe_in[1], body.c_str(), body.size());
    if (bytes_written == -1) {
      perror("write");
      httpErrorCode = 500;
      close(pipe_in[1]);
      close(cgi_fd);
      return -1;
    }
  }
  close(pipe_in[1]);
  int status;
  waitpid(cgi_pid, &status, WNOHANG);
  close(cgi_fd);
  return 0;
}
