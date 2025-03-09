/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 12:25:20 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/09 12:50:44 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Socket.hpp"

Socket::Socket() {}

Socket::~Socket() { close(socket_fd); }

void Socket::setSocketNonBlocking(int fd) {}

void Socket::bindToAddress(int port) {}

void Socket::listenForConnections(int backlog) {}

int Socket::acceptConnection() {}

int Socket::getFd() const {}
