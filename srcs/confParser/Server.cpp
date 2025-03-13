/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdibia <opdibia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 18:01:04 by opdi-bia          #+#    #+#             */
/*   Updated: 2025/03/13 16:25:39 by opdibia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"


Server::Server() {
    method.resize(3, ""); 
}

Server::~Server() {}

std::map<std::string, AnyValue> Server::getConfig() const {
 
  return _server_map;
}

void Server::set_mapValue(const std::string &key, const AnyValue &value) {
    _server_map[key] = value;
}

AnyValue Server::get_mapValue(const std::string &key) const {
    std::map<std::string, AnyValue>::const_iterator it = _server_map.find(key);
    if (it != _server_map.end()) 
        return it->second;
    else
        return AnyValue();
}

void Server::addLocation(const std::string &path, const Location &location) {
    locations[path] = location;
}

void  Server::set_method(int i, const std::string &value) {
    if(i < 0 || i >= 3)
        throw VectorExeption();
    method[i] = value;
}

std::string  Server::get_method(int i) {
    return(method[i]);
}

// void Server::display() const
// {
//     std::cout << "----- Server Config -----" << std::endl;
//     for (std::map<std::string, AnyValue>::const_iterator it = _server_map.begin(); it != _server_map.end(); ++it) {
//         std::cout << it->first << " = " << it->second << std::endl;
//     }
//     std::cout << "-------------------------" << std::endl;
    
//     for (std::map<std::string, Location>::const_iterator it = locations.begin(); it != locations.end(); ++it) {
//         std::cout << "Location: " << it->first << std::endl;
//         it->second.display();
//     }
// }

