/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AnyValue.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdibia <opdibia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 11:36:49 by opdibia           #+#    #+#             */
/*   Updated: 2025/03/16 00:57:37 by opdibia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/AnyValue.hpp"

AnyValue::AnyValue() : _data(NULL), _type("empty") {}  

AnyValue::AnyValue(int val) {
    _data = new int(val);
    _type = "int";
}

AnyValue::AnyValue(double val) {
    _data = new double(val);
    _type = "double";
}

AnyValue::AnyValue(size_t val) {
    _data = new size_t(val);
    _type = "size_t";
}

AnyValue::AnyValue(const std::string& val) {
    _data = new std::string(val);
    _type = "string";
}

AnyValue::AnyValue(const AnyValue& other) {
    _type = other._type;
    if (other._data == NULL)
        _data = NULL;
    else if (other._type == "int") 
        _data = new int(*static_cast<int*>(other._data));
    else if (other._type == "double") 
        _data = new double(*static_cast<double*>(other._data));
    else if (other._type == "size_t") 
        _data = new size_t(*static_cast<size_t*>(other._data));
    else if (other._type == "string") 
        _data = new std::string(*static_cast<std::string*>(other._data));
    else 
        _data = NULL;
}

AnyValue& AnyValue::operator=(const AnyValue& other) {
    if (this == &other) 
        return (*this);
    deleteData();
    _type = other._type;
    if (other._data == NULL)
        _data = NULL;
    else if (other._type == "int") 
        _data = new int(*static_cast<int*>(other._data));
    else if (other._type == "double") 
        _data = new double(*static_cast<double*>(other._data));
    else if (other._type == "size_t") 
        _data = new size_t(*static_cast<size_t*>(other._data));
    else if (other._type == "string") 
        _data = new std::string(*static_cast<std::string*>(other._data));
    else 
        _data = NULL;
    return *this;
}

AnyValue::~AnyValue() {
    deleteData();
}

std::string AnyValue::getType() const { 
    return _type; 
}

int AnyValue::getInt() const {
    if(_type == "int" && _data != NULL) 
        return(*static_cast<int*>(_data));
    else
        return(0);
}

double AnyValue::getDouble() const {
    if(_type == "double" && _data != NULL) 
        return(*static_cast<double*>(_data));
    else
        return(0.0);
}

size_t AnyValue::getSize_t() const {
    if(_type == "size_t" && _data != NULL) 
        return(*static_cast<size_t*>(_data));
    else
        return(0);
}

std::string AnyValue::getString() const {
    if(_type == "string" && _data != NULL) 
        return(*static_cast<std::string*>(_data));
    else
        return("");
}

void AnyValue::deleteData() {
    if (_type == "int") delete static_cast<int*>(_data);
    else if (_type == "double") delete static_cast<double*>(_data);
    else if (_type == "size_t") delete static_cast<size_t*>(_data);
    else if (_type == "string") delete static_cast<std::string*>(_data);
    _data = NULL;
}

bool AnyValue::isEmpty()const {
    return (_data == NULL || _type == "empty");
}
    