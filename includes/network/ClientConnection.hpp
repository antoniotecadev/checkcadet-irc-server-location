/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientConnection.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ateca <ateca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 19:24:57 by ateca             #+#    #+#             */
/*   Updated: 2026/03/11 19:35:07 by ateca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_CONNECTION_HPP
#define CLIENT_CONNECTION_HPP

#include <string>

class ClientConnection
{
private:

    int socketFd;
    std::string inputBuffer;

public:

    ClientConnection(int fd);
    ~ClientConnection();

    int getFd() const;

    void appendBuffer(const std::string &data);
    std::string &getBuffer();

};

#endif