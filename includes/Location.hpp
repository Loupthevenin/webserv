/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdibia <opdibia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 11:06:35 by opdibia           #+#    #+#             */
/*   Updated: 2025/03/13 16:28:25 by opdibia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>
#include <vector>
#include <string>
#include <iostream>

class Location {
private:
    std::map<std::string, std::string> map_location;

public:
    void setValue(const std::string &key, const std::string &value);
    std::string getValue(const std::string &key) const;
    // void display() const;
};