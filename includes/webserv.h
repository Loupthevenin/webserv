/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdibia <opdibia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 12:15:33 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/24 15:22:31 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_H
# define WEBSERV_H

# include "CGIExec.hpp"
# include "ConfigParser.hpp"
# include "Epoll.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
# include "Socket.hpp"
# include "log.hpp"
# include "signal.hpp"
# include <cerrno>
# include <cstddef>
# include <cstdlib>
# include <dirent.h>
# include <iostream>
# include <netinet/in.h>
# include <set>
# include <sys/socket.h>
# include <sys/stat.h>
# include <vector>

// Utils handleRequest
void	sendError(int fd, int statusCode, const std::string &body);
void	closeConnexion(int fd, Epoll &epoll, std::map<int,
			HttpRequest> &requests);
std::string readFile(const std::string &filePath);
std::string listFilesInDirectory(const std::string &directory);
void	setFdNonBlocking(int fd);
int		check_location(Location &location, Server &serverConfig,
			HttpRequest &request, HttpResponse &response, int fd);
std::string find_loc(HttpRequest &request, Server &serverConfig);
int		check_server(HttpRequest &request, HttpResponse &response,
			Server &serverConfig, int fd);
int		check_file(std::string &filePath);
bool	body_size(HttpRequest &request, Location location);
std::string buildErrorResponse(int code);
std::string check_error_server(int code, Server &serverConf);
std::string set_autoindex(const std::string &filePath);
std::string check_header(std::string uri);

// Utils ConfigParser
bool	is_returnNum(const std::string &str);
bool	is_errorNum(const std::string &str);
bool	isNum(const std::string &str);
bool	isValidIPv4(const std::string &ip);
bool	isValidPort(const std::string &port);

// Main
void	handleRequest(int fd, Epoll &epoll, ConfigParser &conf);
int		handleGet(HttpRequest &request, HttpResponse &response,
			Server &serverConfig, int fd);
void	handlePost(HttpRequest &request, HttpResponse &response,
			Server &serverConfig);
void	handleDelete(HttpRequest &request, HttpResponse &response,
			Server &serverConfig);

#endif
