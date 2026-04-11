/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ateca <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 17:10:00 by ateca             #+#    #+#             */
/*   Updated: 2026/04/11 17:10:00 by ateca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/irc/ChannelManager.hpp"

ChannelManager::~ChannelManager()
{
    for (auto const& [name, channel] : channels)
    {
        delete channel;
    }
}

// Retorna canal ou cria um novo
Channel* ChannelManager::getOrCreateChannel(const std::string &name)
{
    auto it = channels.find(name);
    if (it == channels.end())
    {
        Channel* newChannel = new Channel(name);
        channels[name] = newChannel;
        return newChannel;
    }
    return it->second;
}

// Busca canal pelo nome
Channel* ChannelManager::getChannel(const std::string &name)
{
    auto it = channels.find(name);
    if (it != channels.end())
    {
        return it->second;
    }
    return nullptr;
}

// Remove usuário de todos os canais (ex: no QUIT)
void ChannelManager::removeUserFromAllChannels(User* user)
{
    std::vector<std::string> channelsToDelete;
    for (auto const& [name, channel] : channels)
    {
        channel->removeMember(user);
        if (channel->isEmpty()) // Deleta canal se ficar vazio
            channelsToDelete.push_back(name);
    }
    
    for (const auto& name : channelsToDelete)
    {
        delete channels[name];
        channels.erase(name);
    }
}

void ChannelManager::deleteChannelIfEmpty(const std::string &name)
{
    auto it = channels.find(name);
    if (it != channels.end() && it->second->isEmpty())
    {
        delete it->second;
        channels.erase(it);
    }
}
