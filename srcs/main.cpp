/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ateca <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 13:55:17 by ateca             #+#    #+#             */
/*   Updated: 2026/03/21 15:09:03 by ateca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server/server.hpp"

std::atomic<bool> g_running(true);

void signalHandler(int signum)
{
    (void)signum;
    std::cout << "\n[signal received] Exiting..." << std::endl;
    g_running = false;
    // Em servidores de alta performance, costuma-se usar um pipe ou
    // event_fd para avisar o epoll_wait que ele deve acordar e sair.
    // Por enquanto, o exit(0) chamará os destrutores globais/estáticos.
    std::exit(signum);
}

int main(int argc, char **argv)
{
    // 1. Validar argumentos (Porta)
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    int port = std::atoi(argv[1]);
    if (port <= 0 || port > 65535)
    {
        std::cerr << "Error: Invalid port. Use between 1 and 65535." << std::endl;
        return 1;
    }

    // 2. Configurar captura de sinais
    // SIGINT = Ctrl+C | SIGTERM = kill command
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    try
    {
        std::cout << "--- Initiating IRC Server ---" << std::endl;
        std::cout << "Port: " << port << std::endl;

        Server server(port);
        server.start();
    }
    catch (const std::exception &e)
    {
        std::cerr << "CRITICAL ERROR: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}