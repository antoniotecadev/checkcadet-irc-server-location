/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageRouter.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ateca <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 17:15:00 by ateca             #+#    #+#             */
/*   Updated: 2026/04/11 17:15:00 by ateca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_ROUTER_HPP
#define MESSAGE_ROUTER_HPP

#include "User.hpp"
#include "Channel.hpp"
#include "UserRegistry.hpp"
#include <string>

class Server; // Forward declaration

class MessageRouter
{
private:
    Server* server;             // Referência para envio físico
    UserRegistry* userRegistry; // Para encontrar todos os usuários

public:
    MessageRouter(Server* server) : server(server), userRegistry(nullptr) {}
    
    void setUserRegistry(UserRegistry* ur) { userRegistry = ur; }

    void sendToUser(User* user, const std::string &message);
    void sendToChannel(Channel* channel, const std::string &message, User* excludeUser = nullptr);
    void broadcast(const std::string &message, User* excludeUser = nullptr);
};

#endif
