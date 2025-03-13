/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdibia <opdibia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 12:17:37 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/13 16:26:24 by opdibia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ConfigParser.hpp"


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
          currentServer.addLocation(currentLocationPath, currentLocation);
          currentLocation = Location();
        }
        servers.push_back(currentServer);
        currentServer = Server();
        currentLocationPath = "";
      }
    }
    else if(key == "location")
    {
      if(!currentLocationPath.empty())
      {
        currentServer.addLocation(currentLocationPath, currentLocation);
        currentLocation = Location();
      }
      currentLocationPath = value;      
    }
    else if(!currentLocationPath.empty())
      currentLocation.setValue(key, value);
    else if(key == "allow_methods")
      setMethod(currentServer, value);
    else
    {
      if(key == "error_page")
      {
        int found = value.find("/", 0);
        key.insert(10, " ");
        key.insert(11, value, 0, found);
        value.erase(0, found);
      }
      check_value(currentServer, key, value);
      currentServer.set_mapValue(key, value);
    }
  }
  if (!currentLocationPath.empty())
    currentServer.addLocation(currentLocationPath, currentLocation);
  if (!currentServer.get_mapValue("listen").isEmpty())
    servers.push_back(currentServer);

}

bool isValidIPv4(const std::string& ip) {
    std::stringstream ss(ip);
    std::string segment;
    std::vector<int> parts;
    
    while (std::getline(ss, segment, '.')) {
        int num;
        if (!(std::istringstream(segment) >> num) || num < 0 || num > 255) {
            return false;
        }
        parts.push_back(num);
    }
    return (parts.size() == 4); 
}

bool isValidPort(const std::string& port) {
    int num;
    if (!(std::istringstream(port) >> num) || num < 1 || num > 65535) {
        return false;
    }
    return (num); 
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

void ConfigParser::check_value(Server &currentServer, std::string key, std::string value) {
  if(value.empty())
    throw WrongValueExeption("empty key : " + key);
  if(key == "listen")
    check_listen(currentServer, value);
  if(key == "server_name")
    check_serverName(value);
  if(key == "allow_methods")
    check_allowMethod(value);
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
//     std::cout<< i << " i " <<std::endl;
//       servers[i].display();
//   }
// }
