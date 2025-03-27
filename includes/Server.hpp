/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdi-bia <opdi-bia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 18:01:24 by opdi-bia          #+#    #+#             */
/*   Updated: 2025/03/27 15:24:22 by opdi-bia         ###   ########.fr       */
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
  std::map<std::string, Location> _locations; 
  std::vector<std::string> _method; 
  std::vector<std::string> _cgiExt; 
  
public:
  Server();
  Server(const Server& other);
  Server& operator=(const Server& other); 
  ~Server();
  void set_Location(const std::string &path, const Location &location);
  void set_mapValue(const std::string &key, const AnyValue &value);
  void set_method(int i, const std::string &value);
  void  set_cgi_ext(int i, const std::string &value);
  std::string  get_method(int i);
  AnyValue get_mapValue(const std::string &key) const;
  std::map<std::string, Location> getLocation() const;
  std::map<std::string, AnyValue> getConfig() const;
  int get_port();
  std::string get_host();
  size_t get_body_client();
  std::string get_error_page(std::string value);
  std::string get_server_name();
  std::string get_root();
  std::string get_index();
  std::string get_autoindex();
  std::string get_cgi_enable() const;
  std::string get_cgi_extension() const;
  std::string get_cgi_path() const;
  bool is_method(std::string str);
  bool is_cgi_ext(std::string str);

  class VectorExeption : public std:: exception
  {
    public :
    virtual const char* what() const throw()
    {
        return("method limit exceeded");
    }
  };
  class WrongValueExeption : public std::exception 
  {
    private:
        std::string message;

    public:
        explicit WrongValueExeption(const std::string& msg) : message(msg) {}

        virtual const char* what() const throw() {
            return message.c_str();
        }
        virtual ~WrongValueExeption() throw() {} 
  };
  // void display() const;
};
