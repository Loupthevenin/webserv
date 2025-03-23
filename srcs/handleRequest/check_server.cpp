/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_server.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdibia <opdibia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:51:53 by opdibia           #+#    #+#             */
/*   Updated: 2025/03/23 01:11:55 by opdibia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webserv.h"


static bool    body_size(HttpRequest &request, Server &server){
    if(server.get_body_client() != 0)
    {
        size_t maxBodySize = server.get_body_client();
        if (request.getBody().size() > maxBodySize) {
            return(false);
        }
    }
    return(true);
}


static std::string  set_filePath(Server &serverConfig, HttpRequest &request){
    std::string uri = request.getUri();
    std::string filePath = uri;
    
    std::string root = serverConfig.get_root();
    if(!root.empty())
    {
        if(root[root.size() - 1] == '/' && filePath[0] == '/')
            root.erase(root.size() - 1 , 1);
        filePath = root + filePath; 
    }
    return(filePath);
}

static void    set_error(Server &serverConfig, HttpResponse &response, int code){
    response.setStatus(code);
    std::string errorFileServ = check_error_server(code, serverConfig);
    
    if(!errorFileServ.empty())
    {
        std::string root = serverConfig.get_root();
        if(!root.empty())
        {
            if(root[root.size() - 1] == '/' && errorFileServ[0] == '/')
                root.erase(root.size() - 1 , 1);
            else if(root[root.size() - 1] != '/' && errorFileServ[0] != '/')
                root += '/';
            errorFileServ = root + errorFileServ; 
        }
        if(check_file(errorFileServ) == 200)
            response.setBody(readFile(errorFileServ));
    }
    else
        response.setBody(buildErrorResponse(code));
    return;  
}

static void    set_response(Server &server, HttpResponse &response, std::string &filePath){
    std::string extension = check_header(filePath); // verif si extension = "" ?
    response.setHeader("Content_type", extension);
    int code_return = check_file(filePath);
    if(code_return != 200)
    {
        set_error(server, response,code_return);
        return;
    }
    std::string body = readFile(filePath);
    response.setStatus(200);
    response.setBody(body);
}


int    check_server(HttpRequest &request, HttpResponse &response, Server &serverConfig, int fd, Epoll &epoll){
    std::string filePath;
    
    // if(serverConfig.is_method("GET") == false)
    // {
    //    std::string extension = check_header(filePath); // verif si extension = "" ?
    //       response.setHeader("Content_type", extension);
    //       set_error(serverConfig, response, 405);
    //       return;
    // }
    if(body_size(request, serverConfig) == false)
    {
        std::string extension = check_header(filePath); // verif si extension = "" ?
        response.setHeader("Content_type", extension);
        set_error(serverConfig, response, 413);
        return(0); 
    }
    filePath = set_filePath(serverConfig, request);
    if(filePath[filePath.size() - 1] == '/')
    {
        if(!serverConfig.get_index().empty())
            filePath = filePath + serverConfig.get_index();
        else
        {
            if(!serverConfig.get_autoindex().empty() && serverConfig.get_autoindex() == "on")
            {
                std::string body = set_autoindex(filePath);
                if(body.empty())
                {
                    set_error(serverConfig, response, 403);
                    return(0);
                }
                response.setStatus(200);
                response.setBody(body);
                return(0);
            }
            else
            {
                std::string extension = check_header(filePath); // verif si extension = "" ?
                response.setHeader("Content_type", extension);
                set_error(serverConfig, response, 404);
                return(0);
            }
        }
        std::string body = readFile(filePath);
        response.setStatus(200);
        response.setBody(body);
        return(0);
    }
     if(check_header(filePath) == "cgi")
    {
        CGIExec cgi("www/cgi/cgi.sh", request, fd);
      int pipe_fd = cgi.execute();
      if (pipe_fd == -1) {
        // error;
      }
		setFdNonBlocking(pipe_fd);
      epoll.addCGIProcess(pipe_fd, cgi);
      return(1);
    }
    set_response(serverConfig, response, filePath);
    return(0);
}
