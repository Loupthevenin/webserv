/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdibia <opdibia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 11:06:50 by opdibia           #+#    #+#             */
/*   Updated: 2025/03/13 16:25:49 by opdibia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Location.hpp"

void Location::setValue(const std::string &key, const std::string &value) {
    if(key != "}")
        map_location[key] = value;
}

std::string Location::getValue(const std::string &key) const {
    std::map<std::string, std::string>::const_iterator it = map_location.find(key);
    if(it != map_location.end()) 
        return(it->second);
    else
        return ("");
}

// void Location::display() const
// {
//     for (std::map<std::string, std::string>::const_iterator it = map_location.begin(); it != map_location.end(); ++it) {
//         std::cout << it->first << " = " << it->second << std::endl;
//     }
//     std::cout << "---------------------------" << std::endl;
// }
