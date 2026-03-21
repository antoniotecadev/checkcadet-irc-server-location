/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientConnection.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ateca <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 19:26:02 by ateca             #+#    #+#             */
/*   Updated: 2026/03/21 13:25:46 by ateca            ###   ########.fr       */
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