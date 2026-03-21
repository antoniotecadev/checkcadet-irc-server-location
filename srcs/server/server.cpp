/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ateca <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 13:55:21 by ateca             #+#    #+#             */
/*   Updated: 2026/03/21 14:23:44 by ateca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/server/server.hpp"

Server::Server(int port) : port(port), serverSocket(-1), epollFd(-1)
{
    // Reservar espaço para 1024 clientes no mapa para evitar realocações frequentes.
    clients.reserve(1024);
}

Server::~Server()
{
    // O mapa 'clients' será limpo automaticamente ao sair daqui.
    // Como ele usa unique_ptr, os destrutores de ClientConnection
    // serão chamados e eles mesmos fecharão seus respectivos sockets.
    // Fechar socket do servidor e epoll

    if (serverSocket != -1)
        close(serverSocket);
    if (epollFd != -1)
        close(epollFd);
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

// Função para configurar o socket como não bloqueante
void Server::setNonBlocking(int fd)
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
    // EPOLLET = modo edge-triggered (avisa apenas quando o estado muda, não repetidamente)
    // EPOLLHUP = avisar quando cliente desconectar (O fechamento "abrupto" ou total)
    // EPOLLRDHUP = avisar quando cliente desconectar (O fechamento "educado")
    // EPOLLERR = avisar quando ocorrer erro no socket
    epoll_event ev;
    ev.events = EPOLLIN | EPOLLET | EPOLLHUP | EPOLLRDHUP | EPOLLERR;
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
    while (g_running)
    {
        // Esperar eventos
        // epoll_wait: bloqueia, espera eventos, retorna quando algo acontece
        // epollFd = epoll
        // events = array de eventos
        // MAX_EVENTS = máximo de eventos
        // -1 = esperar infinito
        // nfds = número de sockets que tiveram eventos
        int nfds = epoll_wait(epollFd, events, MAX_EVENTS, -1);

        if (nfds == -1)
        {
            // errno == EINTR significa que o epoll_wait foi interrompido por um sinal (como SIGINT)
            if (errno == EINTR)
                continue;
            break;        // Erro real
        }

        for (int i = 0; i < nfds; i++)
        {
            int fd = events[i].data.fd;
            uint32_t ev = events[i].events;

            if (fd == serverSocket)
            {
                // novo cliente
                acceptClient();
            }
            else
            {
                // mensagem de cliente
                // EPOLLERR = erro no socket
                // EPOLLHUP = cliente desconectou (O fechamento "abrupto" ou total)
                // EPOLLRDHUP = cliente desconectou (O fechamento "educado")
                if (ev & (EPOLLERR | EPOLLHUP | EPOLLRDHUP))
                {
                    disconnectClient(fd);
                }
                else
                {
                    // EPOLLIN = há dados para ler
                    if (ev & EPOLLIN)
                        handleRead(fd);

                    // EPOLLOUT = socket pronto para escrever
                    if (ev & EPOLLOUT)
                        handleWrite(fd);
                }
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
                break; // Não há mais conexões para aceitar, sair do loop

            std::cerr << "accept failed" << std::strerror(errno) << std::endl;
            return; // Em caso de erro, apenas retornar (pode ser um erro temporário)
        }

        char ipBuffer[INET_ADDRSTRLEN]; // Espaço para "XXX.XXX.XXX.XXX"

        // 1. Converte o IP de binário para String
        inet_ntop(AF_INET, &clientAddr.sin_addr, ipBuffer, INET_ADDRSTRLEN);

        // 2. Converte a Porta de Network Byte Order para Host Byte Order
        // ntohs (Network To Host Short) = converte a porta de network byte order (big endian) para host byte order (pode ser little endian ou big endian dependendo da arquitetura)
        // uint16_t = tipo de dado para armazenar a porta (16 bits)
        uint16_t port = ntohs(clientAddr.sin_port);

        std::cout << "IP: " << ipBuffer << " | Porta: " << port << std::endl;

        // Configurar o socket do cliente como não bloqueante e adicioná-lo ao epoll
        setNonBlocking(clientFd);
        addClient(clientFd);
    }
}

void Server::addClient(int fd)
{
    // Adicionar cliente no epoll
    // EPOLLIN = avisar quando houver dados para ler
    // EPOLLET = modo edge-triggered (avisa apenas quando o estado muda, não repetidamente)
    // EPOLLHUP = avisar quando cliente desconectar (O fechamento "abrupto" ou total)
    // EPOLLRDHUP = avisar quando cliente desconectar (O fechamento "educado")
    // EPOLLERR = avisar quando ocorrer erro no socket
    epoll_event ev;
    ev.events = EPOLLIN | EPOLLET | EPOLLHUP | EPOLLRDHUP | EPOLLERR;
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
    // std::make_unique<ClientConnection>(fd) = cria um novo ClientConnection e retorna um ponteiro único
    // clients.emplace(fd, std::make_unique<ClientConnection>(fd)) = adiciona o cliente no mapa com a chave fd
    // O mapa clients é do tipo std::map<int, ClientConnection*>, então precisamos criar um ponteiro para ClientConnection
    // emplace é mais eficiente que insert porque evita cópias desnecessárias
    // std::make_unique é uma função que cria um objeto e retorna um std::unique_ptr para ele, garantindo que a memória seja liberada automaticamente quando o ponteiro sair de escopo
    clients.emplace(fd, std::make_unique<ClientConnection>(fd));
    std::cout << "New client connected: " << fd << std::endl;
    sendMessage(fd, ":server 001 Welcome\r\n");
}

void Server::handleRead(int fd)
{
    // Armazena dados recebidos.
    char buffer[4096];

    while (true)
    {
        // Lê dados do socket.
        ssize_t bytes = recv(fd, buffer, sizeof(buffer), 0);

        // bytes == -1 significa que ocorreu um erro ou não há mais dados para ler.
        if (bytes == -1)
        {
            // EAGAIN = não há mais dados para ler
            // EWOULDBLOCK = operação bloqueante, mas socket é não bloqueante
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                break; // Não há mais dados para ler, sair do loop

            disconnectClient(fd);
            return; // Em caso de erro, desconectar cliente e retornar
        }

        // bytes == 0 significa que o cliente desconectou
        if (bytes == 0)
        {
            disconnectClient(fd);
            return;
        }

        // bytes > 0 significa que recebemos dados do cliente
        // auto = deduz o tipo da variável automaticamente
        auto it = clients.find(fd);
        if (it != clients.end())
        {
            // it->second é o seu std::unique_ptr<ClientConnection>
            it->second->appendBuffer(buffer, bytes);
        }
        else
        {
            // Caso o cliente não exista no mapa (erro de lógica ou desconexão)
            std::cerr << "Error: Client " << fd << " not found!" << std::endl;
            return;
        }

        std::cout << "Message from client " << fd << ": " << buffer << std::endl;
    }
}

void Server::disconnectClient(int fd)
{
    // Remover cliente do epoll
    // EPOLL_CTL_DEL = remover um socket do monitoramento
    // nullptr = não precisamos de um evento para remover
    epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, nullptr);

    // Fechar socket do cliente
    close(fd);

    // Remover cliente do mapa
    clients.erase(fd);

    std::cout << "Client disconnected: " << fd << std::endl;
}

void Server::handleWrite(int fd)
{
    auto it = clients.find(fd);
    if (it == clients.end())
        return;

    std::string &out = it->second->getBuffer();

    while (!out.empty())
    {
        // MSG_NOSIGNAL = evitar que o processo receba SIGPIPE se o cliente desconectar durante o envio
        ssize_t bytes = send(fd, out.data(), out.size(), MSG_NOSIGNAL);

        // bytes == -1 significa que ocorreu um erro ou o buffer do kernel está cheio.
        if (bytes > 0)
        {
            // Remover os bytes enviados do buffer de saída.
            out.erase(0, bytes);
        }
        else
        {
            // EAGAIN = buffer do kernel cheio, não podemos enviar mais agora
            // EWOULDBLOCK = operação bloqueante, mas socket é não bloqueante
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                return; // Buffer do kernel cheio, saímos e esperamos o próximo EPOLLOUT

            disconnectClient(fd);
            return;
        }
    }

    // Só removemos o EPOLLOUT quando o buffer interno estiver TOTALMENTE vazio
    modifyEpoll(fd, EPOLLIN | EPOLLET | EPOLLRDHUP);
}

void Server::sendMessage(int fd, const std::string &msg)
{
    auto it = clients.find(fd);
    if (it == clients.end())
        return;

    std::string &out = it->second->getBuffer();

    // Se o buffer já estava vazio, tentamos enviar directo para ganhar velocidade
    if (out.empty())
    {
        // MSG_NOSIGNAL = evitar que o processo receba SIGPIPE se o cliente desconectar durante o envio
        ssize_t bytes = send(fd, msg.data(), msg.size(), MSG_NOSIGNAL);

        if (bytes == -1)
        {
            // EAGAIN = buffer do kernel cheio, não podemos enviar mais agora
            // EWOULDBLOCK = operação bloqueante, mas socket é não bloqueante
            if (errno != EAGAIN && errno != EWOULDBLOCK)
            {
                disconnectClient(fd);
                return;
            }
            // Se deu EAGAIN, guardamos tudo e ligamos o EPOLLOUT
            out.append(msg);
            modifyEpoll(fd, EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP);
        }
        else if ((size_t)bytes < msg.size())
        {
            // Enviou apenas uma parte, guarda o resto e liga o EPOLLOUT
            out.append(msg.substr(bytes));
            modifyEpoll(fd, EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP);
        }
        // Se enviou tudo (bytes == msg.size()), não fazemos nada com o Epoll!
    }
    else
    {
        // Se já havia algo no buffer, apenas adicionamos ao final para manter a ordem
        out.append(msg);
    }
}

void Server::modifyEpoll(int fd, uint32_t events)
{
    epoll_event ev;
    ev.events = events;
    ev.data.fd = fd;

    epoll_ctl(epollFd, EPOLL_CTL_MOD, fd, &ev);
}