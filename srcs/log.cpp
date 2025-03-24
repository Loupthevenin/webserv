/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:13:33 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/24 16:02:42 by ltheveni         ###   ########.fr       */
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

void logInfo(const std::string &type, const std::string &message, const std::string &color) {
	std::cout << getTimestamp() << " " << color << "[" << type << "] " << message << _END << std::endl;
}

void logConnexion(const std::string &ip, int port) {
	std::cout << getTimestamp() << " " << _GREEN << "[CONNEXION] New connexion from: " << ip << ":" << port << _END << std::endl;
}

void logError(const std::string &message) {
	std::cerr << getTimestamp() << " " << _RED << "[ERROR] " << message << _END << std::endl;
}
