/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_CheckLocation.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdi-bia <opdi-bia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 12:11:39 by opdi-bia          #+#    #+#             */
/*   Updated: 2025/03/27 15:30:29 by opdi-bia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webserv.h"

  
bool    check_body_size_loc(HttpRequest &request, Location &location, Server &serverConf){
  if(location.get_body_client() != 0)
  {
      size_t maxBodySize = location.get_body_client();
      if (request.getBody().size() > maxBodySize) {
          return(false);
      }
  }
  else if (serverConf.get_body_client() != 0) {
    size_t maxBodySize = serverConf.get_body_client();
    if (request.getBody().size() > maxBodySize) {
      return (false);
    }
  }
  return(true);
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
  
static std::string join_path(std::string &base, std::string &path) {
    if (base[base.size() - 1] == '/' && path[0] == '/')
      base.erase((base.size() - 1), 1);
    else if (base[base.size() - 1] != '/' && path[0] != '/')
      base += '/';
    path = base + path;
    return (path);
  }
  
int set_error_loc(Server &serverConfig, HttpResponse &response,
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
        int res = try_exec_cgi_loc(location,serverConfig, request, response, fd, errorFileLoc);
        if(res != 0)
          return(res);
      }
    } else if (!errorFileServ.empty()) {
      std::string root = serverConfig.get_root();
      if (!root.empty()) {
        join_path(root, errorFileServ);
      }
      if (check_file(errorFileServ) == 200){
        response.setBody(readFile(errorFileServ));
        response.setHeader("Content-Type", check_header(errorFileServ));
        int res = try_exec_cgi_loc(location,serverConfig, request, response, fd, errorFileServ);
        if(res != 0)
          return(res);
      }
    } else
    {
      response.setHeader("Content-Type", "text/html");
      response.setBody(buildErrorResponse(code));
    }
    return(0);
}
  
  
int  try_exec_cgi_loc(Location &location, Server &serverConfig,
    HttpRequest &request, HttpResponse &response, int fd, std::string filePath ){
    if (check_header(filePath) == "cgi") {
      if(!location.get_cgi_enable().empty() && location.get_cgi_enable() == "on" && location.is_cgiExt(get_ext(filePath)))
      {
        CGIExec cgi(filePath, request, fd);
        if (cgi.execute(request.getBody()) == -1) {
            int error_code = cgi.getHttpErrorCode();
            if(set_error_loc(serverConfig, response, location, error_code, request, fd) == 1)
              return(1);
            return(2);
        }
        return (0);
      }
      else{
        if(!serverConfig.get_cgi_enable().empty() && serverConfig.get_cgi_enable() == "on" && serverConfig.is_cgi_ext(get_ext(filePath)))
        {
          CGIExec cgi(filePath, request, fd);
          if (cgi.execute(request.getBody()) == -1) {
              int error_code = cgi.getHttpErrorCode();
              if(set_error_serv(serverConfig, response, error_code, request, fd) == 1)
                return(1);
              return(2);
          }
          return (0);
        }
      }
      if(set_error_loc(serverConfig, response, location, 403, request, fd) == 1) // a verifier code 403?
        return(1);
      return(2);
    }
    return(0);
}
  
int set_response_loc(Server &server, HttpResponse &response,
                           Location &location, std::string &filePath, HttpRequest &request, int fd) {
    std::string extension = check_header(filePath); 
    int code_return = check_file(filePath);
    if (code_return != 200) {
      if(set_error_loc(server, response, location, code_return, request, fd) == 1)
        return(1);
      return(0);
    }
    response.setHeader("Content-Type", extension);
    std::string body = readFile(filePath);
    response.setStatus(200);
    response.setBody(body);
    return(0);
}
  
