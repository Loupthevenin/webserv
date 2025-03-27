/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_CheckServer.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdi-bia <opdi-bia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 12:06:44 by opdi-bia          #+#    #+#             */
/*   Updated: 2025/03/27 15:29:04 by opdi-bia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webserv.h"

bool check_body_size_serv(HttpRequest &request, Server &serverConf) {
    if (serverConf.get_body_client() != 0) {
        size_t maxBodySize = serverConf.get_body_client();
        if (request.getBody().size() > maxBodySize) {
        return (false);
        }
    }
    return (true);
}
  
std::string set_filePath_serv(Server &serverConfig, HttpRequest &request) {
    std::string uri = request.getUri();
    std::string filePath = uri;
  
    std::string root = serverConfig.get_root();
    if (!root.empty()) {
      if (root[root.size() - 1] == '/' && filePath[0] == '/')
        root.erase(root.size() - 1, 1);
      filePath = root + filePath;
    }
    return (filePath);
}
  
int set_error_serv(Server &serverConfig, HttpResponse &response, int code, HttpRequest &request, int fd) {
    response.setStatus(code);
    std::string errorFileServ = check_error_server(code, serverConfig);
  
    if (!errorFileServ.empty()) {
      std::string root = serverConfig.get_root();
      if (!root.empty()) {
        if (root[root.size() - 1] == '/' && errorFileServ[0] == '/')
          root.erase(root.size() - 1, 1);
        else if (root[root.size() - 1] != '/' && errorFileServ[0] != '/')
          root += '/';
        errorFileServ = root + errorFileServ;  
      }
      if (check_file(errorFileServ) == 200){
        response.setBody(readFile(errorFileServ));
        response.setHeader("Content-Type", check_header(errorFileServ));
        int res = try_exec_cgi_serv(serverConfig, request, response, fd, errorFileServ);
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
  
int  try_exec_cgi_serv(Server &serverConfig,
    HttpRequest &request, HttpResponse &response, int fd, std::string filePath ){
    if (check_header(filePath) == "cgi") {
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
      else{
        if(set_error_serv(serverConfig, response, 403, request, fd) == 1) // a verifier code 403 ?
          return(1);
        return(2);
      }
    }
    return(0);
}
  
int set_response_serv(Server &server, HttpResponse &response,
                           std::string &filePath, HttpRequest &request, int fd) {
    std::string extension = check_header(filePath);
    int code_return = check_file(filePath);
    if (code_return != 200) {
      if(set_error_serv(server, response, code_return, request, fd) == 1)
        return(1);
      return(0);
    }
    response.setHeader("Content-Type", extension);
    std::string body = readFile(filePath);
    response.setStatus(200);
    response.setBody(body);
    return(0);
}