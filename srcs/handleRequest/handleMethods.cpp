/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleMethods.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdi-bia <opdi-bia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 12:24:35 by opdi-bia          #+#    #+#             */
/*   Updated: 2025/03/31 12:59:49 by opdi-bia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webserv.h"

std::string find_loc(HttpRequest &request, Server &serverConfig) {
    std::string uri = request.getUri();
    const std::map<std::string, Location>& locations = serverConfig.getLocation();
    // for (std::map<std::string, Location>::const_iterator it = locations.begin(); it != locations.end(); ++it) {
    //     std::cout << "Location: " << it->first << std::endl;
    //     it->second.display();
    // }
    std::string test = uri;
    while (true) {
        if (locations.find(test) != locations.end())
        {
          // std::cout << "loc: " << test << std::endl;
          return(test);
        }
        if (test == "/" || test.empty())
            break;
        while (!test.empty() && test[test.size() - 1] == '/' && test != "/")
            test = test.substr(0, test.size() - 1);
        size_t slash = test.find_last_of('/');
        if (slash == std::string::npos || slash == 0)
            break;
        test = test.substr(0, slash + 1);
    }
    if (locations.find("/") == locations.end())
    {
      // std::cout << "loc: " << test << std::endl;
        return("/");
    }
    // std::cout << "loc: vide" << std::endl;
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
        if ((location.is_emptyMethods() == false && !location.is_method("POST") )|| (location.is_emptyMethods() == true && !serverConfig.is_method("POST"))) {
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
      filePath = set_filePath_serv(serverConfig, request);
    }
    else
    {
      const std::map<std::string, Location>& locations = serverConfig.getLocation();
      Location location = locations.at(loc);
      if ((location.is_emptyMethods() == false && !location.is_method("DELETE")) || (location.is_emptyMethods() == true && !serverConfig.is_method("DELETE"))) {
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
      filePath = set_filePath_loc(location, loc, serverConfig, request);
    }
    check_dir(response, filePath);
    return(0);
}
