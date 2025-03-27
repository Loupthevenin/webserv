/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_Get.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdi-bia <opdi-bia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:51:53 by opdibia           #+#    #+#             */
/*   Updated: 2025/03/27 14:55:27 by opdi-bia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webserv.h"

static std::string is_return(Location location) {
  std::vector<std::string> returnNum;
  returnNum.push_back("301");
  returnNum.push_back("302");
  returnNum.push_back("303");
  returnNum.push_back("307");
  returnNum.push_back("308");
  for (size_t i = 0; i < returnNum.size(); i++) {
    if (!location.get_return(returnNum[i]).empty())
      return (returnNum[i]);
  }
  return ("");
}

static std::string check_root(Server &serverConfig, Location &location,
  std::string file) {
  std::string root = location.get_root();
  if (!root.empty()) {
    if (root[root.size() - 1] == '/' && file[0] == '/')
      root.erase(root.size() - 1, 1);
    file = root + file;
  } else {
    root = serverConfig.get_root();
    if (!root.empty()) {
      if (root[root.size() - 1] == '/' && file[0] == '/')
        root.erase(root.size() - 1, 1);
      file = root + file;
    }
  }
return (file);
}

std::string set_filePath_loc(Location &location, std::string locName,
  Server &serverConfig, HttpRequest &request) {
  std::string uri = request.getUri();
  std::string filePath = uri;

  if (!location.get_alias().empty()) {
    std::string alias = location.get_alias();
    if (alias[alias.size() - 1] != '/')
      alias += "/";
    size_t pos = uri.find(locName);
    if (pos != std::string ::npos)
      filePath.replace(pos, locName.length(), alias);
  } else
    filePath = check_root(serverConfig, location, filePath);
  return (filePath);
}

int check_location(Location &location, Server &serverConfig,
                   HttpRequest &request, HttpResponse &response, int fd) {
  std::string filePath;

  if (!location.is_method("GET") && !serverConfig.is_method("GET")) {
    std::string extension = check_header(filePath); 
    response.setHeader("Content-Type", extension);
    if(set_error_loc(serverConfig, response, location, 405, request, fd) == 1)
      return(1);
    return (0);
  }
  if (check_body_size_loc(request, location, serverConfig) == false) {
    std::string extension = check_header(filePath); 
    response.setHeader("Content-Type", extension);
    if(set_error_loc(serverConfig, response, location, 413, request, fd) == 1)
      return(1);
    return (0);
  }
  if (!is_return(location).empty()) {
    int code;
    std::istringstream iss(is_return(location));
    iss >> code;
    filePath = location.get_return(is_return(location));
    response.setStatus(code);
    int res = try_exec_cgi_loc(location,serverConfig, request, response, fd, filePath);
    if(res != 0)
      return(res);
    std::string header = check_header(filePath);
    response.setHeader("Content-Type", header);
    response.setBody(buildReturnResponse(code, filePath));
    return (0);
  }
  filePath = set_filePath_loc(location, location.get_nameLoc(), serverConfig, request);
  if (filePath[filePath.size() - 1] == '/') {
    if (!location.get_index().empty())
      filePath = filePath + location.get_index();
    else if (!serverConfig.get_index().empty())
      filePath = filePath + serverConfig.get_index();
    else {
      if (!location.get_autoindex().empty() &&
          location.get_autoindex() == "on") {
        std::string body = set_autoindex(filePath);
        if (body.empty()) {
          if(set_error_loc(serverConfig, response, location, 403, request, fd) == 1)
            return(1);
          return (0);
        }
        response.setStatus(200);
        response.setBody(body);
        return (0);
      } else {
        std::string extension = check_header(filePath);
        response.setHeader("Content-Type", extension);
        if(set_error_loc(serverConfig, response, location, 404, request, fd) == 1)
          return(1);
        return (0);
      }
    }
  }
  int res = try_exec_cgi_loc(location,serverConfig, request, response, fd, filePath);
  if(res != 0)
    return(res);
  if(set_response_loc(serverConfig, response, location, filePath, request, fd) == 1)
    return(1);
  return (0);
}


int check_server(HttpRequest &request, HttpResponse &response,
                 Server &serverConfig, int fd) {
  std::string filePath;

  if(serverConfig.is_method("GET") == false)
  {
     std::string extension = check_header(filePath);
        response.setHeader("Content-Type", extension);
        if(set_error_serv(serverConfig, response, 405, request, fd) == 1)
          return(1);
        return (0);
  }
  if (check_body_size_serv(request, serverConfig) == false) {
    std::string extension = check_header(filePath);
    response.setHeader("Content-Type", extension);
    if(set_error_serv(serverConfig, response, 413, request, fd) == 1)
      return(1);
    return (0);
  }
  filePath = set_filePath_serv(serverConfig, request);
  if (filePath[filePath.size() - 1] == '/') {
    if (!serverConfig.get_index().empty())
      filePath = filePath + serverConfig.get_index();
    else {
      if (!serverConfig.get_autoindex().empty() &&
          serverConfig.get_autoindex() == "on") {
        std::string body = set_autoindex(filePath);
        if (body.empty()) {
          if(set_error_serv(serverConfig, response, 403, request, fd) == 1)
            return(1);
          return (0);
        }
        response.setStatus(200);
        response.setBody(body);
        return (0);
      } else {
        std::string extension =
            check_header(filePath);
        response.setHeader("Content-Type", extension);
        if(set_error_serv(serverConfig, response, 404, request, fd) == 1)
          return(1);
        return (0);
      }
    }
    std::string body = readFile(filePath);
    response.setStatus(200);
    response.setBody(body);
    return (0);
  }
  int res = try_exec_cgi_serv(serverConfig, request, response, fd, filePath);
  if(res != 0)
    return(res);
  if(set_response_serv(serverConfig, response, filePath, request, fd) == 1)
    return(1);
  return (0);
}
