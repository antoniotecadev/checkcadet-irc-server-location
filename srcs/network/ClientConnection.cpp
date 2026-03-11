/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientConnection.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ateca <ateca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 19:26:02 by ateca             #+#    #+#             */
/*   Updated: 2026/03/11 19:29:58 by ateca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/network/ClientConnection.hpp"

ClientConnection::ClientConnection(int fd) : socketFd(fd)
{
}

ClientConnection::~ClientConnection()
{
}

int ClientConnection::getFd() const
{
    return socketFd;
}

void ClientConnection::appendBuffer(const std::string &data)
{
    inputBuffer += data;
}

std::string &ClientConnection::getBuffer()
{
    return inputBuffer;
}