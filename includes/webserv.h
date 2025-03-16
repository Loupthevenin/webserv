/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdibia <opdibia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 12:15:33 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/16 12:15:38 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_H
# define WEBSERV_H

# include "ConfigParser.hpp"
# include "Epoll.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
# include "Socket.hpp"
# include <cerrno>
# include <cstddef>
# include <cstdlib>
# include <dirent.h>
# include <iostream>
# include <netinet/in.h>
# include <set>
# include <sys/socket.h>
# include <vector>

// Utils handleRequest
void	sendError(int fd, int statusCode, const std::string &body);
void	closeConnexion(int fd, Epoll &epoll, std::map<int,
			HttpRequest> &requests);
std::string readFile(const std::string &filePath);
std::string listFilesInDirectory(const std::string &directory);

// Utils ConfigParser
bool	is_returnNum(const std::string &str);
bool	is_errorNum(const std::string &str);
bool	isNum(const std::string &str);
bool	isValidIPv4(const std::string &ip);
bool	isValidPort(const std::string &port);

// Main
void	handleRequest(int fd, Epoll &epoll, ConfigParser &conf);
void	handleGet(HttpRequest &request, HttpResponse &response,
			Server &serverConfig);
void	handlePost(HttpRequest &request, HttpResponse &response,
			Server &serverConfig);
void	handleDelete(HttpRequest &request, HttpResponse &response,
			Server &serverConfig);

#endif
