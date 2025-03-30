/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:23:58 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/30 11:33:16 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/HttpRequest.hpp"
#include <stdexcept>

HttpRequest::HttpRequest()
    : method(""), uri(""), httpVersion(""), contentLength(0),
      headerComplete(false) {}

HttpRequest::~HttpRequest() {}

std::string HttpRequest::getMethod() const { return method; }

std::string HttpRequest::getUri() const { return uri; }

std::string HttpRequest::getHttpVersion() const { return httpVersion; }

std::map<std::string, std::string> HttpRequest::getHeaders() const {
  return headers;
}

std::string HttpRequest::getHeader(const std::string &key) const {
  std::map<std::string, std::string>::const_iterator it = headers.find(key);
  if (it != headers.end())
    return it->second;
  return "";
}

std::string HttpRequest::getBody() const { return body; }

int HttpRequest::getContentLength() const { return contentLength; }

std::string HttpRequest::getHost() const {
  std::map<std::string, std::string>::const_iterator it = headers.find("host");
  if (it != headers.end()) {
    std::string host = it->second;
    if (!host.empty() && host[host.size() - 1] == '\r')
      host.erase(host.size() - 1);
    return host;
  }
  return "";
}

std::string HttpRequest::getQuery() const { return queryString; }

bool HttpRequest::isHeaderComplete() const { return headerComplete; }

bool HttpRequest::hasCompleteHeaders() {
  size_t pos = rawData.find("\r\n\r\n");
  if (pos != std::string::npos) {
    headerComplete = true;
    return true;
  }
  return false;
}

bool HttpRequest::isValidHeader(const std::string &line) {
  std::cout << "Verif Header: '" << line << "'" << std::endl;
  size_t pos = line.find(": ");
  if (pos == std::string::npos)
    return false;

  std::string key = line.substr(0, pos);
  if (key.empty())
    return false;
  return true;
}

bool HttpRequest::isDoubleHeader() {
  std::set<std::string> seenHeaders;

  for (std::map<std::string, std::string>::iterator it = headers.begin();
       it != headers.end(); ++it) {
    const std::string &key = it->first;

    if (seenHeaders.find(key) != seenHeaders.end())
      return true;

    seenHeaders.insert(key);
  }
  return false;
}

void HttpRequest::appendRawData(const std::string &data) {
  rawData += data;

  if (!headerComplete && hasCompleteHeaders())
    parseHeaders();

  if (contentLength > 0) {
    size_t bodyStart = rawData.find("\r\n\r\n") + 4;
    if (bodyStart < rawData.size())
      body = rawData.substr(bodyStart);
  }
}

void HttpRequest::parseHeaders() {
  std::istringstream requestStream(rawData);
  std::string line;

  if (!std::getline(requestStream, line) || line.empty())
    throw std::runtime_error("400 Bad Request: Request line missing");

  if (line[line.size() - 1] == '\r')
    line.erase(line.size() - 1);

  std::istringstream lineStream(line);
  if (!(lineStream >> method >> uri >> httpVersion))
    throw std::runtime_error("400 Bad Request: Invalid request line format");

  if (httpVersion != "HTTP/1.1" && httpVersion != "HTTP/1.0")
    throw std::runtime_error("400 Bad Request: Unsupported HTTP version");

  if (method != "GET" && method != "POST" && method != "DELETE")
    throw std::runtime_error("400 Bad Request: Unsupported HTTP method");

  size_t queryPos = uri.find('?');
  if (queryPos != std::string::npos) {
    std::string tmp = uri.substr(0, queryPos);
    queryString = uri.substr(queryPos + 1);
    uri = tmp;
  } else
    queryString = "";

  bool foundEmptyLine = false;
  while (std::getline(requestStream, line)) {
    if (!line.empty() && line[line.size() - 1] == '\r') {
      line.erase(line.size() - 1);
    }

    if (line.empty()) {
      foundEmptyLine = true;
      break;
    }

    size_t pos = line.find(": ");
    if (pos != std::string::npos) {
      std::string key = line.substr(0, pos);
      std::transform(key.begin(), key.end(), key.begin(), ::tolower);
      std::string value = line.substr(pos + 2);
      headers[key] = value;
    } else {
      throw std::runtime_error("400 Bad Request: Invalid header format");
    }
  }

  if (!foundEmptyLine)
    throw std::runtime_error(
        "400 Bad Request: Missing empty line between headers and body");

  if (headers.find("host") == headers.end() || headers["host"].empty())
    throw std::runtime_error("400 Bad Request: Missing or empty Host header");

  if (isDoubleHeader())
    throw std::runtime_error("400 Bad Request: Double Header");

  contentLength = 0;
  if (headers.find("content-length") != headers.end()) {
    std::stringstream ss(headers["content-length"]);
    ss >> contentLength;
  }
}

void HttpRequest::appendBodyData(const std::string &data) {
  if (contentLength > 0 && body.size() < static_cast<size_t>(contentLength))
    body += data;
}

void HttpRequest::printRequest() const {
  std::cout << "Method: " << method << std::endl;
  std::cout << "URI: " << uri << std::endl;
  std::cout << "Version: " << httpVersion << std::endl;
  std::cout << "Headers:\n";

  std::map<std::string, std::string>::const_iterator it;

  for (it = headers.begin(); it != headers.end(); ++it) {
    std::cout << " " << it->first << ": " << it->second << std::endl;
  }
  std::cout << "Body:\n" << body << std::endl;
}
