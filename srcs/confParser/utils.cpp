/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdibia <opdibia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 14:11:02 by opdibia           #+#    #+#             */
/*   Updated: 2025/03/15 15:02:35 by opdibia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webserv.h"

bool is_returnNum(const std::string& str) {

  if(str.compare("301") && str.compare("302") && str.compare("303") && str.compare("307") && 
      str.compare("308"))
            return(false);
  return(true);
}

bool is_errorNum(const std::string& str) {

  if(str.compare("400") && str.compare("401") && str.compare("403") && str.compare("404") && 
      str.compare("405") && str.compare("408") && str.compare("413") && str.compare("414") && 
        str.compare("429") && str.compare("500") && str.compare("502") && str.compare("503") && str.compare("504"))
            return(false);
  return(true);
}

bool isNum(const std::string& str) {
    if (str.empty()) 
      return false;
    for (size_t i = 0; i < str.length(); i++) 
    {
        if (!std::isdigit(str[i]))
          return false;
    }
    return true;
}

bool isValidIPv4(const std::string& ip) {
    std::stringstream ss(ip);
    std::string segment;
    std::vector<int> parts;
    
    while (std::getline(ss, segment, '.')) {
        int num;
        std::istringstream iss(segment);
        if(!isNum(segment) || !(iss >> num)  || !iss.eof() || num < 0 || num > 255)
            return false;
        parts.push_back(num);
    }
    return (parts.size() == 4); 
}

bool isValidPort(const std::string& port) {
    int num;
    std::istringstream iss(port);
    if(!isNum(port) || !(iss >> num) || !iss.eof() || num < 1 || num > 65535)
        return false;
    return (num); 
}