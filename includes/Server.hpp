/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdibia <opdibia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 18:01:24 by opdi-bia          #+#    #+#             */
/*   Updated: 2025/03/13 16:27:51 by opdibia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../includes/Location.hpp"
#include "../includes/AnyValue.hpp"
#include <map>
#include <string>

class Server {
private:
  std::map<std::string, AnyValue> _server_map;
  std::map<std::string, Location> locations; 
  std::vector<std::string> method; 
  
public:
  Server();
  ~Server();
  void addLocation(const std::string &path, const Location &location);
  void set_mapValue(const std::string &key, const AnyValue &value);
  void  set_method(int i, const std::string &value);
  std::string  get_method(int i);
  AnyValue get_mapValue(const std::string &key) const;
  std::map<std::string, AnyValue> getConfig() const;
  class VectorExeption : public std:: exception
  {
    public :
    virtual const char* what() const throw()
    {
        return("method limit exceeded");
    }
  };
  // void display() const;
};
