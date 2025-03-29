/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:13:33 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/28 11:07:28 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/log.hpp"
#include <ctime>

static std::string getTimestamp() {
  time_t now = time(0);
  struct tm tstruct;
  char buf[80];
  tstruct = *localtime(&now);
  strftime(buf, sizeof(buf), "[%Y-%m-%d %X]", &tstruct);
  return std::string(buf);
}

void logInfo(const std::string &type, const std::string &message,
             const std::string &uri, const std::string &color) {
  std::cout << color << getTimestamp() << " " << "[" << type << "] " << message
            << " " << uri << _END << std::endl;
}

void logConnexion(const std::string &ip, int port) {
  std::cout << _GREEN << getTimestamp() << " "
            << "[CONNEXION] New connexion from: " << ip << ":" << port << _END
            << std::endl;
}

void logError(const std::string &message) {
  std::cerr << _RED << getTimestamp() << " " << "[ERROR] " << message << _END
            << std::endl;
}
