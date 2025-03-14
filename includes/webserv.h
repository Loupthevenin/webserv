/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdibia <opdibia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 12:15:33 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/14 15:02:30 by ltheveni         ###   ########.fr       */
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
# include <iostream>
# include <netinet/in.h>
# include <set>
# include <sys/socket.h>
# include <vector>

// Utils handleRequest
void	sendError(int fd, int statusCode, const std::string &body);
void	closeConnexion(int fd, Epoll &epoll, std::map<int,
			HttpRequest> &requests);

// Main
void	handleRequest(int fd, Epoll &epoll, ConfigParser &conf);
void	handleGet(HttpRequest &request, HttpResponse &response,
			Server &serverConfig);
void	handlePost(HttpRequest &request, HttpResponse &response,
			Server &serverConfig);
void	handleDelete(HttpRequest &request, HttpResponse &response,
			Server &serverConfig);

#endif
