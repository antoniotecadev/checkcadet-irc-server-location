/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ateca <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 13:53:18 by ateca             #+#    #+#             */
/*   Updated: 2026/03/21 11:15:29 by ateca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <unordered_map> // for std::unordered_map
#include <sys/epoll.h>   // for epoll functions and structures
#include <sys/socket.h>  // for socket functions and structures
#include <netinet/in.h>  // for sockaddr_in structure
#include <stdexcept>     // for std::runtime_error
#include <unistd.h>      // for close function
#include <iostream>      // for std::cout and std::cerr
#include <cstring>       // for std::string
#include <fcntl.h>       // for fcntl function
#include <vector>        // for std::vector
#include <cerrno>        // for errno and strerror
#include <string>        // for std::string
#include <memory>        // for std::unique_ptr

#include "../network/ClientConnection.hpp"

class Server
{
private:
    int port;
    int serverSocket;
    int epollFd;

    static const int MAX_EVENTS = 64; // define a constant for maximum events
    struct epoll_event events[MAX_EVENTS];

    // std::unordered_map (Hash): O computador aplica uma função matemática rápida no fd que gera um "endereço" (bucket) directo
public:
    std::unordered_map<int, std::unique_ptr<ClientConnection>> clients;

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
    void setNonBlocking(int fd);
    void disconnectClient(int fd);
};

#endif