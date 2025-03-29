/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleMethods.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdibia <opdibia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 12:24:35 by opdi-bia          #+#    #+#             */
/*   Updated: 2025/03/28 20:27:39 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webserv.h"

std::string find_loc(HttpRequest &request, Server &serverConfig) {
    std::string uri = request.getUri();
    const std::map<std::string, Location>& locations = serverConfig.getLocation();

    std::string test = uri;
    while (true) {
        if (locations.find(test) != locations.end())
            return(test);
        if (test == "/" || test.empty())
            break;
        while (!test.empty() && test[test.size() - 1] == '/' && test != "/")
            test = test.substr(0, test.size() - 1);
        size_t slash = test.find_last_of('/');
        if (slash == std::string::npos || slash == 0)
            break;
        test = test.substr(0, slash + 1);
    }
    if (locations.find("/") != locations.end())
        return("/");
    return("");
}

int	handleGet(HttpRequest &request, HttpResponse &response, Server &serverConfig, int fd){
    
    std::string loc = find_loc(request, serverConfig);
    if(loc.empty())
    {
        if(check_server(request, response, serverConfig, fd) == 1)
            return(1);
    }
    else
    {
        const std::map<std::string, Location>& locations = serverConfig.getLocation();
        Location loc_path = locations.at(loc);
        if(check_location(loc_path, serverConfig, request, response, fd) == 1)
            return(1);
    }
    return(0);
}

int	handlePost(HttpRequest &request, HttpResponse &response, Server &serverConfig, int fd){
    std::cout << "ici dans handle post body request post  : " << request.getUri() << std::endl;
    std::string filePath = request.getUri();
    std::string loc = find_loc(request, serverConfig);
    if(loc.empty())
    {
        if(serverConfig.is_method("POST") == false)
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
        int res = try_exec_cgi_serv(serverConfig, request, response, fd, filePath);
        if(res != 0)
          return(res);
    }
    else
    {
        const std::map<std::string, Location>& locations = serverConfig.getLocation();
        Location location = locations.at(loc);
        if (!location.is_method("POST") && !serverConfig.is_method("POST")) {
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
          filePath = set_filePath_loc(location, location.get_nameLoc(), serverConfig, request);
          int res = try_exec_cgi_loc(location,serverConfig, request, response, fd, filePath);
          if(res != 0)
            return(res);
    }
    if(create_file(request, response) == 1)
        return(1);
    return(0);
}

int handleDelete(HttpRequest &request, HttpResponse &response, Server &serverConfig, int fd) {
    std::string filePath = request.getUri();
    std::string loc = find_loc(request, serverConfig);
    const std::map<std::string, Location>& locations = serverConfig.getLocation();
    Location location = locations.at(loc);
    if(loc.empty())
    {
      if(serverConfig.is_method("DELETE") == false)
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
    }
    else
    {
      if (!location.is_method("DELETE") && !serverConfig.is_method("DELETE")) {
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
    }
    std::string file = request.getUri(); 
    file = set_filePath_loc(location, loc, serverConfig, request);
    check_dir(response, file);
    return(0);
}
