/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ateca <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 13:53:18 by ateca             #+#    #+#             */
/*   Updated: 2026/03/21 15:08:17 by ateca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <unordered_map> // for std::unordered_map
#include <sys/epoll.h>   // for epoll functions and structures
#include <sys/socket.h>  // for socket functions and structures
#include <netinet/in.h>  // for sockaddr_in structure
#include <arpa/inet.h>   // for inet_ntop function
#include <stdexcept>     // for std::runtime_error
#include <unistd.h>      // for close function
#include <iostream>      // for std::cout and std::cerr
#include <cstring>       // for std::string
#include <fcntl.h>       // for fcntl function
#include <vector>        // for std::vector
#include <cerrno>        // for errno and strerror
#include <string>        // for std::string
#include <memory>        // for std::unique_ptr
#include <atomic>        // for std::atomic
#include <csignal>
#include <cstdlib>

#include "../network/ClientConnection.hpp"
#include "../irc/UserRegistry.hpp"
#include "../irc/ChannelManager.hpp"
#include "../irc/MessageRouter.hpp"
#include "../irc/CommandDispatcher.hpp"
#include "../irc/Parser.hpp"

// Variável global atômica para controle de parada segura
// A variável global que o signalHandler vai alterar
// atomic<bool> é uma classe template que fornece operações atômicas para tipos de dados.
// Ela é usada para garantir que as operações de leitura e escrita em uma variável sejam indivisíveis, ou seja, não podem ser interrompidas por outros threads.
// Isso é especialmente importante em situações de concorrência, como no caso de um servidor que pode receber sinais de interrupção (como SIGINT) enquanto está em execução.
// Ao usar std::atomic<bool>, garantimos que a variável g_running seja acessada e modificada de forma segura entre o signalHandler e o loop principal do servidor, evitando condições de corrida e garantindo uma parada ordenada do servidor quando um sinal for recebido.
// extern = a palavra-chave que indica que a variável g_running é definida em outro arquivo (neste caso, srcs/main.cpp) e que estamos apenas declarando sua existência aqui para que possa ser usada no Server class.
extern std::atomic<bool> g_running;

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

private:
    UserRegistry userRegistry;
    ChannelManager channelManager;
    MessageRouter messageRouter;
    CommandDispatcher commandDispatcher;

public:
    Server(int port);
    ~Server();

    void start();
    void sendMessage(int fd, const std::string &msg);

private:
    void setupSocket();
    void setupEpoll();
    void eventLoop();
    void acceptClient();
    void addClient(int fd);
    void handleRead(int fd);
    void handleWrite(int fd);
    void setNonBlocking(int fd);
    void disconnectClient(int fd);
    void modifyEpoll(int fd, uint32_t events);
};

#endif