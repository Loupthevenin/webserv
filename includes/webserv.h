/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 12:15:33 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/13 10:26:11 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_H
# define WEBSERV_H

# include "Epoll.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
# include "Socket.hpp"
# include <cerrno>
# include <cstddef>
# include <cstdlib>
# include <iostream>
# include <netinet/in.h>
# include <sys/socket.h>
# include <vector>

void	handleRequest(int fd, Epoll &epoll);

#endif
