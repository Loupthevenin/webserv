/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdi-bia <opdi-bia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 18:01:04 by opdi-bia          #+#    #+#             */
/*   Updated: 2025/03/27 15:37:29 by opdi-bia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"


Server::Server() {
    _method.resize(3, ""); 
    _method.resize(2, ""); 
}
Server::Server(const Server& other) 
    : _server_map(other._server_map), _locations(other._locations), _method(other._method) {
}

Server& Server::operator=(const Server& other) {
    if (this != &other) {
        _server_map = other._server_map;
        _locations = other._locations;
        _method = other._method;
    }
    return *this;
}

Server::~Server() {}

void Server::set_Location(const std::string &path, const Location &location) {
    if (path[0] != '/')
        throw WrongValueExeption("invalid location " + path);
    _locations[path] = location;
}

void Server::set_mapValue(const std::string &key, const AnyValue &value) {
    _server_map[key] = value;
}

void  Server::set_method(int i, const std::string &value) {
    if(i < 0 || i >= 3)
        throw VectorExeption();
    _method[i] = value;
}

void  Server::set_cgi_ext(int i, const std::string &value) {
    if(i < 0 || i >= 3)
        throw VectorExeption();
    _cgiExt[i] = value;
}

std::string  Server::get_method(int i) {
    if(!_method[i].empty())
        return(_method[i]);
    return("");
}

AnyValue Server::get_mapValue(const std::string &key) const {
    std::map<std::string, AnyValue>::const_iterator it = _server_map.find(key);
    if (it != _server_map.end()) 
        return it->second;
    else
        return AnyValue();
}

std::map<std::string, Location> Server::getLocation() const {
    return _locations;
}

std::map<std::string, AnyValue> Server::getConfig() const {
  return _server_map;
}

int Server::get_port(){
    std::map<std::string, AnyValue>::const_iterator it = _server_map.find("port");
    if (it != _server_map.end()) 
        return (it->second.getInt());
    else
        return (-1);
}

std::string Server::get_host(){
    std::map<std::string, AnyValue>::const_iterator it = _server_map.find("host");
    if (it != _server_map.end()) 
        return (it->second.getString());
    else
        return ("");
}

size_t Server::get_body_client(){
    std::map<std::string, AnyValue>::const_iterator it = _server_map.find("body_size");
    if (it != _server_map.end()) 
        return (it->second.getSize_t());
    else
        return (0);
}

std::string Server::get_error_page(std::string value){
    std::map<std::string, AnyValue>::const_iterator it = _server_map.find(value);
    if (it != _server_map.end()) 
        return (it->second.getString());
    else
        return ("");
}

std::string Server::get_server_name(){
    std::map<std::string, AnyValue>::const_iterator it = _server_map.find("server_name");
    if (it != _server_map.end()) 
        return (it->second.getString());
    else
        return ("");
}

std::string Server::get_root(){
    std::map<std::string, AnyValue>::const_iterator it = _server_map.find("root");
    if (it != _server_map.end()) 
        return (it->second.getString());
    else
        return ("");
}

std::string Server::get_index(){
    std::map<std::string, AnyValue>::const_iterator it = _server_map.find("index");
    if (it != _server_map.end()) 
        return (it->second.getString());
    else
        return ("");
}

std::string Server::get_autoindex(){
    std::map<std::string, AnyValue>::const_iterator it = _server_map.find("autoindex");
    if (it != _server_map.end()) 
        return (it->second.getString());
    else
        return ("");
}

std::string Server::get_cgi_enable() const{
    std::map<std::string, AnyValue>::const_iterator it = _server_map.find("cgi_enable");
    if (it != _server_map.end()) 
        return (it->second.getString());
    else
        return ("");
}

std::string Server::get_cgi_extension() const{
    std::map<std::string, AnyValue>::const_iterator it = _server_map.find("cgi_extension");
    if (it != _server_map.end()) 
        return (it->second.getString());
    else
        return ("");
}

std::string Server::get_cgi_path() const{
    std::map<std::string, AnyValue>::const_iterator it = _server_map.find("cgi_path");
    if (it != _server_map.end()) 
        return (it->second.getString());
    else
        return ("");
}

bool    Server::is_method(std::string str){
    for(int i = 0; i < 3; i++)
    {
        if(_method[i] == str)
            return(true);
    }
    return(false);
}

bool    Server::is_cgi_ext(std::string str){
    for(int i = 0; i < 2; i++)
    {
        if(_cgiExt[i] == str)
            return(true);
    }
    return(false);
}

// void Server::display() const
// {
//     std::cout << "----- Server Config -----" << std::endl;
//     for (std::map<std::string, AnyValue>::const_iterator it = _server_map.begin(); it != _server_map.end(); ++it) {
//         std::cout << it->first << " = " << it->second << std::endl;
//     }
//     std::cout << "-------------------------" << std::endl;
    
//     for (std::map<std::string, Location>::const_iterator it = _locations.begin(); it != _locations.end(); ++it) {
//         std::cout << "Location: " << it->first << std::endl;
//         it->second.display();
//     }
// }

