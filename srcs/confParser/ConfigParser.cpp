/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdibia <opdibia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 12:17:37 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/16 01:09:12 by opdibia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ConfigParser.hpp"
#include "../../includes/webserv.h"


ConfigParser::ConfigParser(const std::string &filename) {
  int len;
  
  len = filename.size();
  if(filename.compare(len - 5, 5, ".conf") != 0)
    throw ConfExeption();  
  _infile.open(filename.c_str());
  if (!_infile.is_open())
      throw OpenExeption();
}

ConfigParser::~ConfigParser() {
  _infile.close();
}

void ConfigParser::parseConfig() {
  std::string buffer;
  std::string value;
  std::string key;
  Server currentServer;
  Location currentLocation;
  std::string currentLocationPath = "";
  
  while(_infile >> key)
  {
    getline(_infile, value);
    if (!value.empty() && value[0] == ' ')
      value.erase(0, 1);  
    std::size_t found = value.find("{");
    if (found!=std::string::npos)
      value.erase(found);
    found = value.find(";");
    if (found!=std::string::npos)
      value.erase(found);
    if(key == "server")
    {
      if(!currentServer.get_mapValue("listen").isEmpty())
      { 
        if(!currentLocationPath.empty())
        {
          currentServer.set_Location(currentLocationPath, currentLocation);
          currentLocation = Location();
        }
        check_isNameServer(currentServer);
        servers.push_back(currentServer);
        currentServer = Server();
        currentLocationPath = "";
      }
    }
    else if(key == "location")
    {
      if(!currentLocationPath.empty())
      {
        currentServer.set_Location(currentLocationPath, currentLocation);
        currentLocation = Location();
      }
      currentLocationPath = value;      
    }
    else if(!currentLocationPath.empty())
      currentLocation.setValue(key, value);
    else if(key == "allow_methods")
      setMethod(currentServer, value);
    else if(key == "error_page")
        set_errorPage(currentServer, value);
    else
    {
      check_value(currentServer, key, value);
      currentServer.set_mapValue(key, value);
    }
  }
  if (!currentLocationPath.empty())
    currentServer.set_Location(currentLocationPath, currentLocation);
  if (!currentServer.get_mapValue("listen").isEmpty())
  {
    check_isNameServer(currentServer);
    servers.push_back(currentServer);
  }
  else  
    throw WrongValueExeption("listen missing");
}

bool  ConfigParser::check_name(std::string value)
{
  for (size_t i = 0; i < servers.size(); ++i) 
  {
      if(value.compare(servers[i].get_mapValue("server_name").getString()) == 0)
        return(false);
  }  
  return(true);
}

void ConfigParser::check_isNameServer(Server &currentServer)
{ 
  int i = 0;
  std::ostringstream oss;
  oss << i; 
  std::string default_name = "default_server_" + oss.str();
  
  if(currentServer.get_mapValue("server_name").isEmpty())
  {
    while(!check_name(default_name))
    {
      i++;
      oss.str("");
      oss.clear();
      oss << i; 
      std::cout << "i = " << oss.str();
      default_name = "default_server_" + oss.str();
    }
    currentServer.set_mapValue("server_name", default_name);
  }
}

void  ConfigParser::set_errorPage(Server &currentServer, std::string value){
  int found; 
  std::string num_error;
  
  found = value.find("/", 0);
  num_error.assign(value, 0, found - 1);
  value.erase(0, found);
  if(!isNum(num_error) || !is_errorNum(num_error))
    throw WrongValueExeption("invalid num error_page " + num_error);
  currentServer.set_mapValue(num_error, value);
}

void ConfigParser::check_listen(Server &currentServer, std::string value) {
  std::string ip;
  std::string default_ip = "0.0.0.0";
  std::string port;
  int num_port;
  std::size_t found;
  found = value.find(":");
  if(found!= std::string::npos)
  {
    ip.assign(value, 0, found);
    value.erase(0, found);
    port.assign(value);
    port.erase(0, 1);
  }
  else{
    if(isValidIPv4(value))
      ip.assign(value);
    else if(isValidPort(value))
      port.assign(value);
    else 
      throw WrongValueExeption("invalid listen " + value);
  }
  if(!ip.empty())
  {
    if(isValidIPv4(ip) == false)
      throw WrongValueExeption("invalid ip " + value);
    currentServer.set_mapValue("host", ip);
  }
  else 
    currentServer.set_mapValue("host", default_ip);
  if(!port.empty())
  {
    if(isValidPort(port) == false)
      throw WrongValueExeption("invalid port " + value);
    std::istringstream(port) >> num_port;
    currentServer.set_mapValue("port", num_port);
  }
  else  
    currentServer.set_mapValue("port", 80);
}

void ConfigParser::check_serverName(std::string value) {
  for (size_t i = 0; i < servers.size(); ++i) {
      if(value.compare(servers[i].get_mapValue("server_name").getString()) == 0)
        throw WrongValueExeption(value + " is already use");
  }
}

void ConfigParser::check_allowMethod(std::string value) {
  if(value.compare("GET") && value.compare("POST") && value.compare("DELETE"))
    throw WrongValueExeption("invalid method " + value);
}

void  ConfigParser::check_autoindex(std::string value){
  if(value.compare("on") && value.compare("off"))
    throw WrongValueExeption("invalid autoindex " + value);
}

void ConfigParser::check_body_size(Server &currentServer, std::string value)
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
      throw WrongValueExeption("invalid client_body_size " + value);
    }
    res = x*1024;
    currentServer.set_mapValue("body_size", res);
  }
  else if(foundM != std::string::npos)
  {
    value.erase(value.begin() + foundM);
    std::istringstream iss(value);
    if(! isNum(value) || !(iss >> x)  || !iss.eof())
    {
      throw WrongValueExeption("invalid client_body_size " + value);
    }
    res = x*1024*1024;
    currentServer.set_mapValue("body_size", res);
  }
  else if(foundG != std::string::npos)
  {
    value.erase(value.begin() + foundG);
    std::istringstream iss(value);
    if(! isNum(value) || !(iss >> x)  || !iss.eof())
      throw WrongValueExeption("invalid client_body_size " + value);
    res = x * 1024 * 1024 * 1024;
    currentServer.set_mapValue("body_size", res);
  }
  else
  {
    std::istringstream iss(value);
    if(! isNum(value) || !(iss >> x)  || !iss.eof())
      throw WrongValueExeption("invalid client_body_size " + value);
    currentServer.set_mapValue("body_size", x);
  }
}

void ConfigParser::check_value(Server &currentServer, std::string key, std::string value) {
  if(value.empty())
    throw WrongValueExeption("empty key : " + key);
  if(key == "listen")
    check_listen(currentServer, value);
  if(key == "server_name")
    check_serverName(value);
  if(key == "allow_methods")
    check_allowMethod(value);
  if(key == "autoindex")
    check_autoindex(value);
  if(key == "client_max_body_size")
    check_body_size(currentServer, value);
}

void  ConfigParser::setMethod(Server &c_server, std::string value) {
  std::istringstream iss(value);
  std::string word;
  int i = 0;

  while(iss >> word)
  {
    check_allowMethod(word);
    c_server.set_method(i, word);
    i++;
  }
}

// void  ConfigParser::display_config() const
// {
//   std::cout << "size server = " << servers.size() <<  std::endl; 
//   for (size_t i = 0; i < servers.size(); ++i) {
//       servers[i].display();
//   }
// }
