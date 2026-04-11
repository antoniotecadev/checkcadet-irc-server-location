/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientConnection.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ateca <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 19:26:02 by ateca             #+#    #+#             */
/*   Updated: 2026/03/21 14:07:53 by ateca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/network/ClientConnection.hpp"

ClientConnection::ClientConnection(int fd) : socketFd(fd)
{
}

ClientConnection::~ClientConnection()
{
    if (socketFd != -1)
    {
        close(socketFd);
        std::cout << "Socket " << socketFd << " closed by the object's destructor." << std::endl;
    }
}

int ClientConnection::getFd() const
{
    return socketFd;
}

void ClientConnection::appendBuffer(const char *data, size_t length)
{
    // Adiciona exatamente 'length' bytes ao final da string.
    inputBuffer.append(data, length);
}

std::string &ClientConnection::getBuffer()
{
    return inputBuffer;
}

void ClientConnection::sendMessage(const std::string &msg)
{
    // Adiciona a mensagem ao buffer de saída do cliente.
    // O buffer de saída é uma string que armazena as mensagens que serão enviadas para o cliente.
    outBuffer += msg;
}

std::string &ClientConnection::getOutBuffer()
{
    return outBuffer;
}