/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdibia <opdibia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 12:17:51 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/13 16:29:53 by opdibia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../includes/Server.hpp"
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <exception>
#include <stdexcept>
#include <sstream>

class Server;

class ConfigParser {
private:
  std::ifstream _infile;
  
public:
  std::vector<Server> servers;
  ConfigParser(const std::string &filename);
  ~ConfigParser();
  void parseConfig();
  void check_value(Server &currentServer, std::string key, std::string value);
  void check_listen(Server &currentServer, std::string value);
  void check_serverName(std::string value);
  void check_allowMethod(std::string value);
  void  setMethod(Server &c_server, std::string value);
  class ConfExeption : public std:: exception
  {
    public :
    virtual const char* what() const throw()
    {
        return("Not .conf extension");
    }
  };
  class OpenExeption : public std:: exception
  {
    public :
    virtual const char* what() const throw()
    {
       return("Failed to open input file .conf");
    }
  };
  class WrongValueExeption : public std::exception {
    private:
        std::string message;

    public:
        explicit WrongValueExeption(const std::string& msg) : message(msg) {}

        virtual const char* what() const throw() {
            return message.c_str();
        }
        virtual ~WrongValueExeption() throw() {} 
  };
    //  void  display_config() const;
};
