/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:09:56 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/24 15:29:21 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <ctime>

#define _GREY	"\033[30m"
#define _RED	"\033[31m"
#define _GREEN	"\033[32m"
#define _YELLOW	"\033[33m"
#define _BLUE	"\033[34m"
#define _PURPLE	"\033[35m"
#define _CYAN	"\033[36m"
#define _WHITE	"\033[37m"
#define _END	"\033[0m"

void logInfo(const std::string &type, const std::string &message, const std::string &color);
void logConnexion(const std::string &ip, int port);
void logError(const std::string &message);
