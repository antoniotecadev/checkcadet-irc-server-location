/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ateca <ateca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 13:53:18 by ateca             #+#    #+#             */
/*   Updated: 2026/03/11 19:18:19 by ateca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/epoll.h>  // for epoll functions and structures
#include <sys/socket.h> // for socket functions and structures
#include <netinet/in.h> // for sockaddr_in structure
#include <unistd.h>     // for close function
#include <stdexcept>    // for std::runtime_error
#include <vector>       // for std::vector
#include <cstring>       // for std::string
#include <iostream>

class Server
{
private:
    int port;
    int serverSocket;
    int epollFd;

    static const int MAX_EVENTS = 64; // define a constant for maximum events
    struct epoll_event events[MAX_EVENTS];

public:
    Server(int port);
    ~Server();

    void start();

private:
    void setupSocket();
    void setupEpoll();
    void eventLoop();
};

#endif