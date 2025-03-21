/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdibia <opdibia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 10:03:21 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/15 14:22:31 by opdibia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <map>
#include <sstream>

class HttpResponse {
private:
	int statusCode;
	std::string statusMessage;
	std::map<std::string, std::string> headers;
	std::string body;

	std::string getStatusMessage(int code) const;
public:
	HttpResponse();
	~HttpResponse();

	void setStatus(int code);
	void setHeader(const std::string &key, const std::string &value);
	void setBody(const std::string &bodyContent);

	std::string toString() const;
};
