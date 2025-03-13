/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opdibia <opdibia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 12:15:33 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/13 16:36:35 by opdibia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_H
# define WEBSERV_H

# include "Epoll.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
# include "Socket.hpp"
# include "ConfigParser.hpp"
# include <cerrno>
# include <cstddef>
# include <cstdlib>
# include <iostream>
# include <netinet/in.h>
# include <sys/socket.h>
# include <vector>

void	handleRequest(int fd, Epoll &epoll);

#endif
