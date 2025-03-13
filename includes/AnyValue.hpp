/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AnyValue.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdibia <opdibia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 11:36:57 by opdibia           #+#    #+#             */
/*   Updated: 2025/03/13 16:30:32 by opdibia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

class AnyValue{
private:
    void *data;
    std::string type;

public:
    AnyValue();
    AnyValue(int val);
    AnyValue(double val);
    AnyValue(const std::string& val);
    ~AnyValue();
    AnyValue& operator=(const AnyValue& other);
    AnyValue(const AnyValue& other);
    void deleteData();
    std::string getType() const;
    int getInt() const;
    double getDouble() const;
    std::string getString() const;
    bool isEmpty()const;
    friend std::ostream& operator<<(std::ostream& os, const AnyValue& value) {
        if(value.isEmpty())
            os << "[empty]";
        else if (value.type == "int")
            os << *static_cast<int*>(value.data);
        else if (value.type == "double")
            os << *static_cast<double*>(value.data);
        else if (value.type == "string")
            os << *static_cast<std::string*>(value.data);
        else
            os << "[Unknown Type]";
        return os;
    }
};