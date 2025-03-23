/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AnyValue.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdibia <opdibia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 11:36:57 by opdibia           #+#    #+#             */
/*   Updated: 2025/03/23 00:48:30 by opdibia          ###   ########.fr       */
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
    void *_data;
    std::string _type;

public:
    AnyValue();
    AnyValue(int val);
    AnyValue(double val);
    AnyValue(size_t val);
    AnyValue(const std::string& val);
    AnyValue(const AnyValue& other);
    AnyValue& operator=(const AnyValue& other);
    ~AnyValue();
    std::string getType() const;
    int getInt() const;
    double getDouble() const;
    size_t getSize_t() const;
    std::string getString() const;
    void deleteData();
    bool isEmpty()const;
    friend std::ostream& operator<<(std::ostream& os, const AnyValue& value) {
        if(value.isEmpty())
            os << "[empty]";
        else if (value._type == "int")
            os << *static_cast<int*>(value._data);
        else if (value._type == "double")
            os << *static_cast<double*>(value._data);
        else if (value._type == "size_t")
            os << *static_cast<size_t*>(value._data);
        else if (value._type == "string")
            os << *static_cast<std::string*>(value._data);
        else
            os << "[Unknown Type]";
        return os;
    }
};