/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:23:58 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/14 15:06:22 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/HttpRequest.hpp"

HttpRequest::HttpRequest() : method(""), uri(""), httpVersion(""), contentLength(0), headerComplete(false) {}

HttpRequest::~HttpRequest() {}

std::string HttpRequest::getMethod() const {
	return method;
}

std::string HttpRequest::getUri() const {
	return uri;
}

std::string HttpRequest::getHttpVersion() const {
	return httpVersion;
}

std::map<std::string, std::string> HttpRequest::getHeaders() const {
	return headers;
}

std::string HttpRequest::getHeader(const std::string &key) const {
std::map<std::string, std::string>::const_iterator it = headers.find(key);
	if (it != headers.end())
		return it->second;
	return "";
}

std::string HttpRequest::getBody() const {
	return body;
}

int HttpRequest::getContentLength() const {
	return contentLength;
}

std::string HttpRequest::getHost() const {
	std::map<std::string, std::string>::const_iterator it = headers.find("Host");
	if (it != headers.end())
	{
		std::string host = it->second;
		if (!host.empty() && host[host.size() - 1] == '\r')
			host.erase(host.size() - 1);
		return host;
	}
	return "";
}

const char *HttpRequest::getBuffer() const {
	return buffer;
}

void HttpRequest::setBuffer(const char *data, size_t size)
{
	if (size < sizeof(buffer))
	{
		memcpy(buffer, data, size);
		buffer[size] = '\0';
	}
	else {
		std::cerr << "Buffer overflow" << std::endl;
	}
}

bool HttpRequest::isHeaderComplete() const{
	return headerComplete;
}

bool HttpRequest::hasCompleteHeaders() {
	size_t pos = rawData.find("\r\n\r\n");
	if (pos != std::string::npos)
	{
		headerComplete = true;
		return true;
	}
	return false;
}

void HttpRequest::appendRawData(const std::string &data) {
	rawData += data;

	if (!headerComplete && hasCompleteHeaders())
		parseHeaders();

	if (contentLength > 0)
	{
		size_t bodyStart = rawData.find("\r\n\r\n") + 4;
		if (bodyStart < rawData.size())
			body = rawData.substr(bodyStart);
	}
}

void HttpRequest::parseHeaders() {
	std::istringstream requestStream(rawData);
	std::string line;

	if (std::getline(requestStream, line)) {
		if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);
		std::istringstream lineStream(line);
		lineStream >> method >> uri >> httpVersion;
	}

	while (std::getline(requestStream, line) && line != "\r")
	{
		if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);

		size_t pos = line.find(": ");
		if (pos != std::string::npos)
	{
			std::string key = line.substr(0, pos);
			std::string value = line.substr(pos + 2);
			headers[key] = value;
		}
	}

	contentLength = 0;
	if (headers.find("Content-Length") != headers.end())
	{
		std::stringstream ss(headers["Content-Length"]);
		ss >> contentLength;
	}
}

void HttpRequest::appendBodyData(const std::string &data)
{
	if (contentLength > 0 && body.size() < static_cast<size_t>(contentLength))
		body += data;
}

void HttpRequest::printRequest() const {
	std::cout << "Method: " << method << std::endl;
	std::cout << "URI: " << uri << std::endl;
	std::cout << "Version: " << httpVersion << std::endl;
	std::cout << "Headers:\n";

	std::map<std::string, std::string>::const_iterator it;

	for (it = headers.begin(); it != headers.end(); ++it)
	{
	  std::cout << " " << it->first << ": " << it->second << std::endl;
	}
	std::cout << "Body:\n" << body << std::endl;
}
