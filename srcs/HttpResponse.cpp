/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdibia <opdibia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 10:03:11 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/23 01:07:31 by opdibia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/HttpResponse.hpp"
#include <sstream>

HttpResponse::HttpResponse() : statusCode(200), statusMessage("OK") {}

HttpResponse::~HttpResponse() {}

std::string HttpResponse::getStatusMessage(int code) const{
	switch (code) {
		case 200: return "OK";
		case 201: return "Created";
		case 204: return "No Content";
		case 400: return "Bad Request";
		case 403: return "Forbidden";
		case 404: return "Not Found";
		case 500: return "Internal Server Error";
		case 501: return "Not Implemented";
		default: return "Unknown";
	}
}

int HttpResponse::getStatus() {
	return(statusCode);
}

std::string HttpResponse::getBody() {
	return(body);
}

void HttpResponse::setStatus(int code) {
	statusCode = code;
	statusMessage = getStatusMessage(code);
}

void HttpResponse::setHeader(const std::string &key, const std::string &value) {
	headers[key] = value;
}

void HttpResponse::setBody(const std::string &bodyContent) {
	body = bodyContent;
	std::stringstream ss;
	ss << body.size();
	setHeader("Content-Length", ss.str());
}

std::string HttpResponse::toString() const {
	std::ostringstream responseStream;
	responseStream << "HTTP/1.1 " << statusCode << " " << statusMessage << "\r\n";

	for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it) {
		responseStream << it->first << ": " << it->second << "\r\n";
	}

	responseStream << "\r\n" << body;
	return responseStream.str();
}
