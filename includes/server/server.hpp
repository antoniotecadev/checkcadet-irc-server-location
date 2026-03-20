/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ateca <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 13:53:18 by ateca             #+#    #+#             */
/*   Updated: 2026/03/20 19:52:48 by ateca            ###   ########.fr       */
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
#include <cstring>      // for std::string
#include <iostream>     // for std::cout and std::cerr
#include <cerrno>       // for errno and strerror
#include <fcntl.h>      // for fcntl function
#include <string>       // for std::string
#include <memory>       // for std::unique_ptr
#include <map>          // for std::map


#include "../network/ClientConnection.hpp"

class Server
{
private:
    int port;
    int serverSocket;
    int epollFd;

    static const int MAX_EVENTS = 64; // define a constant for maximum events
    struct epoll_event events[MAX_EVENTS];

public:
    std::map<int, ClientConnection *> clients;

public:
    Server(int port);
    ~Server();

    void start();

private:
    void setupSocket();
    void setupEpoll();
    void eventLoop();
    void acceptClient();
    void addClient(int fd);
    void handleClient(int fd);

};

#endif