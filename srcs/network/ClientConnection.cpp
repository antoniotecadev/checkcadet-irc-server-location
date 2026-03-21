/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientConnection.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ateca <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 19:26:02 by ateca             #+#    #+#             */
/*   Updated: 2026/03/21 11:33:10 by ateca            ###   ########.fr       */
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

void ClientConnection::appendBuffer(const std::string &data, ssize_t length)
{
    // Adiciona os dados recebidos ao buffer de entrada do cliente.
    // O buffer é uma string que armazena os dados recebidos do cliente.
    // append = adiciona os dados ao final da string
    // data = ponteiro para os dados recebidos
    // length = número de bytes recebidos
    inputBuffer.append(data, length);
}

std::string &ClientConnection::getBuffer()
{
    return inputBuffer;
}