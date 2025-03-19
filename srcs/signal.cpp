/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltheveni <ltheveni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 17:56:47 by ltheveni          #+#    #+#             */
/*   Updated: 2025/03/19 18:57:28 by ltheveni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/signal.hpp"
#include <iostream>

volatile bool g_running = true;

void signalHandler(int sig)
{
	if (sig == SIGINT)
	{
		std::cout << "\nReceived SIGINT, shutting down..." << std::endl;
		g_running = false;
	}
}
