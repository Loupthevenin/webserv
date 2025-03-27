/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdi-bia <opdi-bia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:24:13 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/27 11:57:57 by opdi-bia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <map>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <cstring>

class HttpRequest {
private:
	std::string rawData;
	std::string method;
	std::string uri;
	std::string httpVersion;
	std::map<std::string, std::string> headers;
	std::string body;
	int contentLength;
	bool headerComplete;
	std::string queryString;

public:
	HttpRequest();
	~HttpRequest();

	std::string getMethod() const;
	std::string getUri() const;
	std::string getHttpVersion() const;
	std::map<std::string, std::string> getHeaders() const;
	std::string getHeader(const std::string &key) const;
	std::string getBody() const;
	int getContentLength() const;
	std::string getHost() const;
	std::string getQuery() const;

	bool isHeaderComplete() const;
	bool hasCompleteHeaders();
	void appendRawData(const std::string &data);
	void parseHeaders();
	void appendBodyData(const std::string &data);

	void printRequest() const;
};
