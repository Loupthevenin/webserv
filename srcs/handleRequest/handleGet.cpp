/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleGet.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdibia <opdibia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:28:17 by opdi-bia          #+#    #+#             */
/*   Updated: 2025/03/23 01:15:12 by opdibia          ###   ########.fr       */
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

int	handleGet(HttpRequest &request, HttpResponse &response, Server &serverConfig, int fd, Epoll &epoll){
    
    std::string loc = find_loc(request, serverConfig);
    // std::cout <<"uri ici = " << request.getUri() << std::endl;
    // std::cout <<"loc ici = " << loc << std::endl;
    if(loc.empty())
    {
        if(check_server(request, response, serverConfig, fd, epoll) == 1)
            return(1);
    }
    else
    {
        const std::map<std::string, Location>& locations = serverConfig.getLocation();
        Location loc_path = locations.at(loc);
        if(check_location(loc_path, serverConfig, request, response, fd, epoll) == 1)
            return(1);
    }
    return(0);
}
