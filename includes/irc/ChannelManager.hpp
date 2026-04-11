/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ateca <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 17:05:00 by ateca             #+#    #+#             */
/*   Updated: 2026/04/11 17:05:00 by ateca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_MANAGER_HPP
#define CHANNEL_MANAGER_HPP

#include "Channel.hpp"
#include <unordered_map>
#include <string>

class ChannelManager
{
private:
    std::unordered_map<std::string, Channel*> channels;

public:
    ~ChannelManager();

    Channel* getOrCreateChannel(const std::string &name);
    Channel* getChannel(const std::string &name);
    void removeUserFromAllChannels(User* user);
    void deleteChannelIfEmpty(const std::string &name);
};

#endif
