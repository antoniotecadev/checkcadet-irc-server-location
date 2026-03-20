/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ateca <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 13:55:21 by ateca             #+#    #+#             */
/*   Updated: 2026/03/20 19:57:34 by ateca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/server/server.hpp"

Server::Server(int port) : port(port), serverSocket(-1), epollFd(-1)
{
}

Server::~Server()
{
}

// Função para configurar o socket como não bloqueante
void setNonBlocking(int fd)
{
    // F_GETFL = get file status flags
    // F_SETFL = set file status flags
    // 0 = não há flags adicionais
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
        throw std::runtime_error("fcntl get failed");

    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
        throw std::runtime_error("fcntl set failed");
}

void Server::setupSocket()
{
    // AF_INET = ipv4
    // SOCK_STREAM = tcp
    // 0 = protocolo automático
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    setNonBlocking(serverSocket);

    if (serverSocket < 0)
        throw std::runtime_error("socket failed");

    // Isso permite reusar a porta imediatamente após reiniciar o servidor.
    // Sem isso receberia: bind: Address already in use
    // Porque o kernel mantém a porta em estado TIME_WAIT.
    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Essa estrutura representa endereço IPv4.
    // sin_family	tipo de endereço
    // sin_port	porta
    // sin_addr	IP
    // htons (big endian) = Host To Network Short | 6667 → convertido para network order
    // INADDR_ANY = 0.0.0.0 | O servidor aceita conexões de qualquer interface.
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    // socket é associado a uma porta. | socket → 0.0.0.0:8080
    if (bind(serverSocket, (sockaddr *)&addr, sizeof(addr)) < 0)
        throw std::runtime_error("bind failed");

    // socket vira socket de escuta | clientes podem conectar
    // SOMAXCONN = máximo de conexões pendentes.
    if (listen(serverSocket, SOMAXCONN) < 0)
        throw std::runtime_error("listen failed");
}

void Server::setupEpoll()
{

    // epoll é um multiplexador de eventos do Linux.
    // Ele permite monitorar milhares de sockets sem threads.

    // Kernel cria uma estrutura interna.
    epollFd = epoll_create1(0);

    if (epollFd < 0)
        throw std::runtime_error("epoll create failed");

    // Criar evento | qual evento que queremos observar
    // EPOLLLIN = avisar quando houver dados para ler
    epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = serverSocket;

    // Registrar no epoll
    // EPOLL_CTL_ADD = adicionar um novo socket para monitorar
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverSocket, &ev) < 0)
        throw std::runtime_error("epoll_ctl failed");

    // Agora epoll está observando: serverSocket
}

void Server::eventLoop()
{
    // coração do servidor.
    while (true)
    {
        // Esperar eventos
        // epoll_wait: bloqueia, espera eventos, retorna quando algo acontece
        // epollFd = epoll
        // events = array de eventos
        // MAX_EVENTS = máximo de eventos
        // -1 = esperar infinito
        // nfds = número de sockets que tiveram eventos
        int nfds = epoll_wait(epollFd, events, MAX_EVENTS, -1);

        for (int i = 0; i < nfds; i++)
        {
            int fd = events[i].data.fd;

            if (fd == serverSocket)
            {
                // novo cliente
                acceptClient();
            }
            else
            {
                // mensagem de cliente
                handleClient(fd);
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

void Server::acceptClient()
{
    while (true)
    {
        // Estrutura do cliente
        // Vai armazenar: IP do cliente, porta do cliente
        sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);

        // Criar evento do cliente
        int clientFd = accept(serverSocket, (sockaddr *)&clientAddr, &clientLen);

        // Se não houver mais clientes para aceitar, sair do loop
        if (clientFd == -1)
        {
            // EAGAIN = não há mais conexões para aceitar
            // EWOULDBLOCK = operação bloqueante, mas socket é não bloqueante
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                break;

            std::cerr << "accept failed" << std::strerror(errno) << std::endl;
            break;
        }

        setNonBlocking(clientFd);

        // Adicionar cliente no epoll
        epoll_event ev;
        ev.events = EPOLLIN;
        ev.data.fd = clientFd;

        handleClient(clientFd);
    }
}

void Server::addClient(int fd)
{
    // Adicionar cliente no epoll
    // EPOLLIN = avisar quando houver dados para ler
    // EPOLLRDHUP = avisar quando cliente desconectar
    // EPOLLERR = avisar quando ocorrer erro no socket
    epoll_event ev;
    ev.events = EPOLLIN | EPOLLRDHUP | EPOLLERR;
    ev.data.fd = fd;

    // Adicionar cliente no epoll
    // EPOLL_CTL_ADD = adicionar um novo socket para monitorar
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &ev) == -1)
    {
        close(fd);
        return;
    }

    // Agora epoll também monitora o cliente.

    // Criar objeto ClientConnection e armazenar no mapa
    clients.emplace(fd, std::make_unique<ClientConnection>(fd));
    std::cout << "New client connected: " << fd << std::endl;
}

void Server::handleClient(int fd)
{
    // Armazena dados recebidos.
    char buffer[512];

    // Lê dados do socket.
    int bytes = recv(fd, buffer, sizeof(buffer), 0);

    if (bytes <= 0)
    {
        // Fechar socket
        close(fd);

        // Deletar objecto
        delete clients[fd];
        clients.erase(fd);

        std::cout << "Client disconnected: " << fd << std::endl;
        return;
    }

    // Transforma em string.
    buffer[bytes] = '\0';

    // Guarda dados incompletos.
    clients[fd]->appendBuffer(buffer);
    std::cout << "Message from client " << fd << ": " << buffer << std::endl;
}