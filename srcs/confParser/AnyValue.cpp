/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AnyValue.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdibia <opdibia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 11:36:49 by opdibia           #+#    #+#             */
/*   Updated: 2025/03/13 16:25:05 by opdibia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/AnyValue.hpp"

AnyValue::AnyValue() : data(NULL), type("empty") {}  

AnyValue::AnyValue(int val) {
    data = new int(val);
    type = "int";
}

AnyValue::AnyValue(double val) {
    data = new double(val);
    type = "double";
}

AnyValue::AnyValue(const std::string& val) {
    data = new std::string(val);
    type = "string";
}

AnyValue::~AnyValue() {
    deleteData();
}

AnyValue::AnyValue(const AnyValue& other) {
    type = other.type;
    if (other.type == "int") 
        data = new int(*static_cast<int*>(other.data));
    else if (other.type == "double") 
        data = new double(*static_cast<double*>(other.data));
    else if (other.type == "string") 
        data = new std::string(*static_cast<std::string*>(other.data));
    else 
        data = NULL;
}

AnyValue& AnyValue::operator=(const AnyValue& other) {
    if (this == &other) 
        return (*this);
    deleteData();
    type = other.type;
    if (other.type == "int") 
        data = new int(*static_cast<int*>(other.data));
    else if (other.type == "double") 
        data = new double(*static_cast<double*>(other.data));
    else if (other.type == "string") 
        data = new std::string(*static_cast<std::string*>(other.data));
    else 
        data = NULL;
    return *this;
}

std::string AnyValue::getType() const { 
    return type; 
}

int AnyValue::getInt() const {
    if(type == "int") 
        return(*static_cast<int*>(data));
    else
        return(0);
}

double AnyValue::getDouble() const {
    if(type == "double") 
        return(*static_cast<double*>(data));
    else
        return(0.0);
}

std::string AnyValue::getString() const {
    if(type == "string") 
        return(*static_cast<std::string*>(data));
    else
        return("");
}

bool AnyValue::isEmpty()const {
    return (data == NULL || type == "empty");
}

void AnyValue::deleteData() {
    if (type == "int") delete static_cast<int*>(data);
    else if (type == "double") delete static_cast<double*>(data);
    else if (type == "string") delete static_cast<std::string*>(data);
    data = NULL;
}
    