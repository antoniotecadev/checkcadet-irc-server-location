/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserRegistry.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ateca <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 16:55:00 by ateca             #+#    #+#             */
/*   Updated: 2026/04/11 16:55:00 by ateca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/irc/UserRegistry.hpp"

UserRegistry::~UserRegistry()
{
    for (auto const& [fd, user] : usersByFd)
    {
        delete user;
    }
}

void UserRegistry::addUser(int fd)
{
    if (usersByFd.find(fd) == usersByFd.end())
    {
        usersByFd[fd] = new User(fd);
    }
}

void UserRegistry::removeUser(int fd)
{
    auto it = usersByFd.find(fd);
    if (it != usersByFd.end())
    {
        User* user = it->second;
        if (!user->getNickname().empty())
        {
            usersByNickname.erase(user->getNickname());
        }
        usersByFd.erase(it);
        delete user;
    }
}

User* UserRegistry::getUserByFd(int fd)
{
    auto it = usersByFd.find(fd);
    if (it != usersByFd.end())
    {
        return it->second;
    }
    return nullptr;
}

User* UserRegistry::getUserByNickname(const std::string &nickname)
{
    auto it = usersByNickname.find(nickname);
    if (it != usersByNickname.end())
    {
        return it->second;
    }
    return nullptr;
}

void UserRegistry::updateNickname(User* user, const std::string &newNickname)
{
    if (!user->getNickname().empty())
    {
        usersByNickname.erase(user->getNickname());
    }
    user->setNickname(newNickname);
    if (!newNickname.empty())
    {
        usersByNickname[newNickname] = user;
    }
}

bool UserRegistry::isNicknameTaken(const std::string &nickname) const
{
    return usersByNickname.find(nickname) != usersByNickname.end();
}
