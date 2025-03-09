/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 12:17:51 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/09 12:25:05 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <string>

class ConfigParser {
private:
  std::map<std::string, std::string> config_map;

public:
  ConfigParser(const std::string &filename);
  ~ConfigParser();

  std::map<std::string, std::string> getConfig() const;
};
