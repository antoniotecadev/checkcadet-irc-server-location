/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageRouter.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ateca <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 17:20:00 by ateca             #+#    #+#             */
/*   Updated: 2026/04/11 17:20:00 by ateca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/irc/MessageRouter.hpp"
#include "../../includes/server/server.hpp"

void MessageRouter::sendToUser(User* user, const std::string &message)
{
    if (user)
    {
        server->sendMessage(user->getFd(), message + "\r\n");
    }
}

void MessageRouter::sendToChannel(Channel* channel, const std::string &message, User* excludeUser)
{
    if (!channel) return;
    
    const std::vector<User*> &members = channel->getMembers();
    for (User* user : members)
    {
        if (user != excludeUser)
        {
            sendToUser(user, message);
        }
    }
}

void MessageRouter::broadcast(const std::string &message, User* excludeUser)
{
    if (!userRegistry) return;
    
    std::vector<User*> all = userRegistry->getAllUsers();
    for (User* user : all)
    {
        if (user != excludeUser)
        {
            sendToUser(user, message);
        }
    }
}
