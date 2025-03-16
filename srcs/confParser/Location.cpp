/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdibia <opdibia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 11:06:50 by opdibia           #+#    #+#             */
/*   Updated: 2025/03/16 01:12:04 by opdibia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Location.hpp"
#include "../../includes/webserv.h"


Location::Location(){
    _method.resize(3, ""); 
}

Location::Location(const Location& other) : _method(other._method){
    for (std::map<std::string, AnyValue>::const_iterator it = other._map_location.begin();
         it != other._map_location.end(); ++it) {
        _map_location[it->first] = AnyValue(it->second);
    }
}

Location& Location::operator=(const Location& other) {
    if (this != &other) {
        _map_location.clear(); 
        _method = other._method;
        for (std::map<std::string, AnyValue>::const_iterator it = other._map_location.begin();
             it != other._map_location.end(); ++it) {
            _map_location[it->first] = AnyValue(it->second);
        }
    }
    return *this;
}

Location::~Location(){}

void  Location::set_method(int i, const std::string &value) {
    if(i < 0 || i >= 3)
        throw VectorExeption();
    _method[i] = value;
}

void Location::setValue(const std::string &key, const AnyValue &value) {
    if(key != "}")
    {
        check_value(key, value);
        _map_location[key] = value;
    }
}

void  Location::setMethod(std::string value) {
  std::istringstream iss(value);
  std::string word;
  int i = 0;

  while(iss >> word)
  {
    check_allowMethod(word);
    set_method(i, word);
    i++;
  }
}

std::string  Location::get_method(int i)  const {
  if(!_method[i].empty())
      return(_method[i]);
  return("");
}

AnyValue Location::getValue(const std::string &key) const {
    std::map<std::string, AnyValue>::const_iterator it = _map_location.find(key);
    if(it != _map_location.end()) 
        return(it->second);
    else
        return AnyValue();
}

size_t Location::get_body_client() const{
    std::map<std::string, AnyValue>::const_iterator it = _map_location.find("body_size");
    if (it != _map_location.end()) 
        return (it->second.getSize_t());
    else
        return (0);
}

std::string Location::get_error_page(std::string value) const{
    std::map<std::string, AnyValue>::const_iterator it = _map_location.find(value);
    if (it != _map_location.end()) 
        return (it->second.getString());
    else
        return ("");
}

std::string Location::get_root() const{
    std::map<std::string, AnyValue>::const_iterator it = _map_location.find("root");
    if (it != _map_location.end()) 
        return (it->second.getString());
    else
        return ("");
}

std::string Location::get_alias() const{
    std::map<std::string, AnyValue>::const_iterator it = _map_location.find("alias");
    if (it != _map_location.end()) 
        return (it->second.getString());
    else
        return ("");
}

std::string Location::get_index() const{
    std::map<std::string, AnyValue>::const_iterator it = _map_location.find("index");
    if (it != _map_location.end()) 
        return (it->second.getString());
    else
        return ("");
}

std::string Location::get_autoindex() const{
    std::map<std::string, AnyValue>::const_iterator it = _map_location.find("autoindex");
    if (it != _map_location.end()) 
        return (it->second.getString());
    else
        return ("");
}

std::string Location::get_return(std::string value) const{
    std::map<std::string, AnyValue>::const_iterator it = _map_location.find(value);
    if (it != _map_location.end()) 
        return (it->second.getString());
    else
        return ("");
}

std::string Location::get_cgi_enable() const{
    std::map<std::string, AnyValue>::const_iterator it = _map_location.find("cgi_enable");
    if (it != _map_location.end()) 
        return (it->second.getString());
    else
        return ("");
}

std::string Location::get_cgi_extension() const{
    std::map<std::string, AnyValue>::const_iterator it = _map_location.find("cgi_extension");
    if (it != _map_location.end()) 
        return (it->second.getString());
    else
        return ("");
}

std::string Location::get_cgi_path() const{
    std::map<std::string, AnyValue>::const_iterator it = _map_location.find("cgi_path");
    if (it != _map_location.end()) 
        return (it->second.getString());
    else
        return ("");
}

void Location::check_value(const std::string &key, const AnyValue&value){
    if(value.isEmpty())
        throw WrongValueExeption("location : empty key : " + key);
    else if(key == "allowed__methods")
        setMethod(value.getString());
    else if(key == "autoindex")
        check_autoindex(value.getString());
    else if(key == "client_max_body_size")
        check_body_size(value.getString());
    else if(key == "error_page")
        check_errorPage(value.getString());
    else if(key == "return")
        check_return(value.getString());
    else if(key == "cgi_extension")
        check_cgi_ext(value.getString());
    else if(key == "cgi_enable")
        check_cgi_enable(value.getString());
}

void Location::check_allowMethod(std::string value) {
  if(value.compare("GET") && value.compare("POST") && value.compare("DELETE"))
    throw WrongValueExeption("location : invalid _method " + value);
}

void  Location::check_autoindex(std::string value){
  if(value.compare("on") && value.compare("off"))
    throw WrongValueExeption("location : invalid autoindex " + value);
}

void Location::check_body_size(std::string value)
{
  size_t foundK;
  size_t foundM;
  size_t foundG;
  size_t res;
  size_t x = 0;
  
  foundK = value.find("K");
  foundM = value.find("M");
  foundG = value.find("G");
  if(foundK != std::string::npos)
  {
    value.erase(value.begin() + foundK);
    std::istringstream iss(value);
    if(! isNum(value) || !(iss >> x)  || !iss.eof())
    {
      throw WrongValueExeption("location : invalid client_body_size " + value);
    }
    res = x*1024;
    _map_location["body_size"] = res;
  }
  else if(foundM != std::string::npos)
  {
    value.erase(value.begin() + foundM);
    std::istringstream iss(value);
    if(! isNum(value) || !(iss >> x)  || !iss.eof())
    {
      throw WrongValueExeption("location : invalid client_body_size " + value);
    }
    res = x*1024*1024;
    _map_location["body_size"] = res;

  }
  else if(foundG != std::string::npos)
  {
    value.erase(value.begin() + foundG);
    std::istringstream iss(value);
    if(! isNum(value) || !(iss >> x)  || !iss.eof())
      throw WrongValueExeption("location : invalid client_body_size " + value);
    res = x * 1024 * 1024 * 1024;
    _map_location["body_size"] = res;

  }
  else
  {
    std::istringstream iss(value);
    if(! isNum(value) || !(iss >> x)  || !iss.eof())
      throw WrongValueExeption("location : invalid client_body_size " + value);
    _map_location["body_size"] = x;
  }
}

void  Location::check_errorPage(std::string value){
  int found; 
  std::string num_error;
  
  found = value.find("/", 0);
  num_error.assign(value, 0, found - 1);
  value.erase(0, found);
  if(!isNum(num_error) || !is_errorNum(num_error))
    throw WrongValueExeption("location : invalid num error_page " + num_error);
  _map_location[num_error] = value;
}

void  Location::check_return(std::string value){
  int found; 
  std::string num_error;
  
  found = value.find("/", 0);
  num_error.assign(value, 0, found - 1);
  value.erase(0, found);
  if(!isNum(num_error) || !is_returnNum(num_error))
    throw WrongValueExeption("location : invalid num return " + num_error);
  _map_location[num_error] = value;
}

void  Location::check_cgi_ext(std::string value){
  if(value.compare(".php"))
    throw WrongValueExeption("location : invalid cgi_extension " + value);
}

void  Location::check_cgi_enable(std::string value){
  if(value.compare("off") && value.compare("on"))
    throw WrongValueExeption("location : invalid cgi_enable " + value);
}

// void Location::display() const
// {
//     for (std::map<std::string, AnyValue>::const_iterator it = _map_location.begin(); it != _map_location.end(); ++it) {
//         std::cout << it->first << " = " << it->second << std::endl;
//     }
//     std::cout << "---------------------------" << std::endl;
// }
