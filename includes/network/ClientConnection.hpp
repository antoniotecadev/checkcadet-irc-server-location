/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientConnection.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ateca <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 19:24:57 by ateca             #+#    #+#             */
/*   Updated: 2026/03/21 14:06:50 by ateca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_CONNECTION_HPP
#define CLIENT_CONNECTION_HPP

#include <string>
#include <unistd.h>      // for close function
#include <iostream>      // for std::cout

class ClientConnection
{
private:
    int socketFd;
    std::string outBuffer;
    std::string inputBuffer;

public:
    ClientConnection(int fd);
    ~ClientConnection();

    int getFd() const;

    void appendBuffer(const std::string &data, ssize_t length);
    std::string &getBuffer();

    void sendMessage(const std::string &msg);
    std::string &getOutBuffer();
};

#endif