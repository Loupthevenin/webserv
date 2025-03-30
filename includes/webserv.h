/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdibia <opdibia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 12:15:33 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/30 10:57:19 by ltheveni         ###   ########.fr       */
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

// handleMethods
int		handleGet(HttpRequest &request, HttpResponse &response,
			Server &serverConfig, int fd);
int		handlePost(HttpRequest &request, HttpResponse &response,
			Server &serverConfig, int fd);
int		handleDelete(HttpRequest &request, HttpResponse &response,
			Server &serverConfig, int fd);

// Utils Post
int		create_file(HttpRequest &request, HttpResponse &response);
bool	isValidFilename(const std::string &name);

// Utils Get
int		check_location(Location &location, Server &serverConfig,
			HttpRequest &request, HttpResponse &response, int fd);
std::string find_loc(HttpRequest &request, Server &serverConfig);
int		check_server(HttpRequest &request, HttpResponse &response,
			Server &serverConfig, int fd);

// UtilsDelete
void	check_dir(HttpResponse &response, std::string filePath);

// Utils handleRequest
void	sendError(int fd, int statusCode, const std::string &body);
void	closeConnexion(int fd, Epoll &epoll, std::map<int,
			HttpRequest> &requests);
std::string readFile(const std::string &filePath);
std::string listFilesInDirectory(const std::string &directory);
void	setFdNonBlocking(int fd);
int		check_file(std::string &filePath);
std::string buildErrorResponse(int code);
std::string check_error_server(int code, Server &serverConf);
std::string set_autoindex(const std::string &filePath);
std::string check_header(std::string uri);
std::string buildReturnResponse(int code, std::string &url);
std::string get_ext(std::string uri);
int		getServerPort(int client_fd);
std::string getServerIp(int client_fd);

// Utils check-server
int		set_response_serv(Server &server, HttpResponse &response,
			std::string &filePath, HttpRequest &request, int fd);
int		try_exec_cgi_serv(Server &serverConfig, HttpRequest &request,
			HttpResponse &response, int fd, std::string filePath);
int		set_error_serv(Server &serverConfig, HttpResponse &response, int code,
			HttpRequest &request, int fd);
bool	check_body_size_serv(HttpRequest &request, Server &serverConf);
std::string set_filePath_serv(Server &serverConfig, HttpRequest &request);
std::string set_filePath_loc(Location &location, std::string locName,
		Server &serverConfig, HttpRequest &request);

// Utils check-location
//  std::string set_filePath_loc(Location &location, std::string locName,
//  Server &serverConfig, HttpRequest &request);
int		set_response_loc(Server &server, HttpResponse &response,
			Location &location, std::string &filePath, HttpRequest &request,
			int fd);
int		try_exec_cgi_loc(Location &location, Server &serverConfig,
			HttpRequest &request, HttpResponse &response, int fd,
			std::string filePath);
int		set_error_loc(Server &serverConfig, HttpResponse &response,
			Location &location, int code, HttpRequest &request, int fd);
bool	check_body_size_loc(HttpRequest &request, Location &location,
			Server &serverConfig);

// Utils ConfigParser
bool	is_returnNum(const std::string &str);
bool	is_errorNum(const std::string &str);
bool	isNum(const std::string &str);
bool	isValidIPv4(const std::string &ip);
bool	isValidPort(const std::string &port);

// Main
void	handleRequest(int fd, Epoll &epoll, ConfigParser &conf);
// int		handleGet(HttpRequest &request, HttpResponse &response,
// 			Server &serverConfig, int fd);
// void	handlePost(HttpRequest &request, HttpResponse &response,
// 			Server &serverConfig);
// void	handleDelete(HttpRequest &request, HttpResponse &response,
// 			Server &serverConfig);

#endif
