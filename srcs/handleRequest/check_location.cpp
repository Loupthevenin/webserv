/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_location.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdibia <opdibia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:51:49 by opdibia           #+#    #+#             */
/*   Updated: 2025/03/25 20:56:43 by opdibia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webserv.h"
#include <sys/stat.h>

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

static std::string check_error_loc(int code, Location &location) {
  std::ostringstream oss;
  oss << code;
  std::string c_error = oss.str();
  std::string err_page = location.get_error_page(c_error);
  if (!err_page.empty())
    return (err_page);
  return ("");
}

static std::string buildReturnResponse(int code, std::string &url) {
  std::ostringstream oss;
  oss << "HTTP/1.1 " << code << " ";

  switch (code) {
  case 301:
    oss << "Moved Permanently";
    break;
  case 302:
    oss << "Found";
    break;
  case 303:
    oss << "See Other";
    break;
  case 307:
    oss << "Temporary Redirect";
    break;
  case 308:
    oss << "Permanent Redirect";
    break;
  default:
    oss << "Error";
    break;
  }

  std::string body = "<html><body><h1>" + oss.str() + "</h1></body></html>";
  oss << body;
  oss << "Location: " << url << "\r\n";
  return (oss.str());
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

static std::string join_path(std::string &base, std::string &path) {
  if (base[base.size() - 1] == '/' && path[0] == '/')
    base.erase((base.size() - 1), 1);
  else if (base[base.size() - 1] != '/' && path[0] != '/')
    base += '/';
  path = base + path;
  return (path);
}

static int set_error(Server &serverConfig, HttpResponse &response,
                      Location &location, int code, HttpRequest &request, int fd) {
  response.setStatus(code);
  std::string errorFileLoc = check_error_loc(code, location);
  std::string errorFileServ = check_error_server(code, serverConfig);

  if (!errorFileLoc.empty()) {
    std::string root = location.get_root();
    if (!root.empty()) {
      join_path(root, errorFileLoc);
    }
    if (check_file(errorFileLoc) == 200){
      response.setBody(readFile(errorFileLoc));
      response.setHeader("Content-Type", check_header(errorFileLoc));
      if(try_exec_cgi(location,serverConfig, request, response, fd, errorFileLoc))
        return(1);
    }
  } else if (!errorFileServ.empty()) {
    std::string root = serverConfig.get_root();
    if (!root.empty()) {
      join_path(root, errorFileServ);
    }
    if (check_file(errorFileServ) == 200){
      response.setBody(readFile(errorFileServ));
      response.setHeader("Content-Type", check_header(errorFileServ));
      if(try_exec_cgi(location,serverConfig, request, response, fd, errorFileServ))
        return(1);
    }
  } else
  {
    response.setHeader("Content-Type", "text/html");
    response.setBody(buildErrorResponse(code));
  }
  return(0);
}


bool  try_exec_cgi(Location &location, Server &serverConfig,
  HttpRequest &request, HttpResponse &response, int fd, std::string filePath ){
  if (check_header(filePath) == "cgi") {
    if(!location.get_cgi_enable().empty() && location.get_cgi_enable() == "on")
    {
      CGIExec cgi(filePath, request, fd);
      if (cgi.execute(request.getBody()) == -1) {
          // leaks !!!!! + no response;
          int error_code = cgi.getHttpErrorCode();
          response.setHeader("Content-Type", "text/html");
          if(set_error(serverConfig, response, location, error_code, request, fd) == 1)
            return(true);
        }
        return (true);
    }
    return(false);
  }
  return(false);
}

static int set_response(Server &server, HttpResponse &response,
                         Location &location, std::string &filePath, HttpRequest &request, int fd) {
  std::string extension = check_header(filePath); 
  int code_return = check_file(filePath);
  if (code_return != 200) {
    if(set_error(server, response, location, code_return, request, fd) == 1)
      return(1);
    return(0);
  }
  response.setHeader("Content-Type", extension);
  std::string body = readFile(filePath);
  response.setStatus(200);
  response.setBody(body);
  return(0);
}

static std::string set_filePath(Location &location, std::string locName,
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
    if(set_error(serverConfig, response, location, 405, request, fd) == 1)
      return(1);
    return (0);
  }
  if (body_size(request, location) == false) {
    std::string extension = check_header(filePath); 
    response.setHeader("Content-Type", extension);
    if(set_error(serverConfig, response, location, 413, request, fd) == 1)
      return(1);
    return (0);
  }
  if (!is_return(location).empty()) {
    int code;
    std::istringstream iss(is_return(location));
    iss >> code;
    filePath = location.get_return(is_return(location));
    response.setStatus(code);
    if(try_exec_cgi(location,serverConfig, request, response, fd, filePath))
      return(1);
    std::string header = check_header(filePath);
    response.setHeader("Content-Type", header);
    response.setBody(buildReturnResponse(code, filePath));
    return (0);
  }
  filePath =
      set_filePath(location, location.get_nameLoc(), serverConfig, request);
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
          if(set_error(serverConfig, response, location, 403, request, fd) == 1)
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
        if(set_error(serverConfig, response, location, 404, request, fd) == 1)
          return(1);
        return (0);
      }
    }
  }
  if(try_exec_cgi(location,serverConfig, request, response, fd, filePath))
    return(1);
  if(set_response(serverConfig, response, location, filePath, request, fd) == 1)
    return(1);
  return (0);
}
