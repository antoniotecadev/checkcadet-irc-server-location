/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ateca <ateca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 13:55:21 by ateca             #+#    #+#             */
/*   Updated: 2026/03/11 19:19:18 by ateca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/server/server.hpp"

Server::Server(int port) : port(port), serverSocket(-1), epollFd(-1)
{
}

Server::~Server()
{
}

void Server::setupSocket()
{
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket < 0)
        throw std::runtime_error("socket failed");

    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (sockaddr *)&addr, sizeof(addr)) < 0)
        throw std::runtime_error("bind failed");

    if (listen(serverSocket, SOMAXCONN) < 0)
        throw std::runtime_error("listen failed");
}

void Server::setupEpoll()
{
    epollFd = epoll_create1(0);

    if (epollFd < 0)
        throw std::runtime_error("epoll create failed");

    epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = serverSocket;

    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverSocket, &ev) < 0)
        throw std::runtime_error("epoll_ctl failed");
}

void Server::eventLoop()
{
    while (true)
    {
        int nfds = epoll_wait(epollFd, events, MAX_EVENTS, -1);

        for (int i = 0; i < nfds; i++)
        {
            int fd = events[i].data.fd;

            if (fd == serverSocket)
            {
                // novo cliente
                // acceptClient();
                std::cout << "New client connected\n";
            }
            else
            {
                // mensagem de cliente
                // handleClient(fd);
                 std::cout << "Message from client\n";
            }
        }
    }
}

void Server::start()
{
    setupSocket();
    setupEpoll();
    eventLoop();
}