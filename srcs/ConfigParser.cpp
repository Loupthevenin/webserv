/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 12:17:37 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/10 13:48:10 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ConfigParser.hpp"

ConfigParser::ConfigParser(const std::string &filename) {
  // PARSING
  (void)filename;
}

ConfigParser::~ConfigParser() {}

std::map<std::string, std::string> ConfigParser::getConfig() const {
  return config_map;
}
