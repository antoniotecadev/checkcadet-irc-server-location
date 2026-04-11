/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserRegistry.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ateca <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 16:50:00 by ateca             #+#    #+#             */
/*   Updated: 2026/04/11 16:50:00 by ateca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_REGISTRY_HPP
#define USER_REGISTRY_HPP

#include "User.hpp"
#include <unordered_map>
#include <string>
#include <vector>

class UserRegistry
{
private:
    std::unordered_map<int, User*> usersByFd;               // Busca via socket
    std::unordered_map<std::string, User*> usersByNickname; // Busca via nome

public:
    ~UserRegistry();
    
    void addUser(int fd);
    void removeUser(int fd);
    
    User* getUserByFd(int fd);
    User* getUserByNickname(const std::string &nickname);
    
    void updateNickname(User* user, const std::string &newNickname);
    
    bool isNicknameTaken(const std::string &nickname) const;
    
    std::vector<User*> getAllUsers() const
    {
        std::vector<User*> all;
        for (auto const& [fd, user] : usersByFd)
        {
            all.push_back(user);
        }
        return all;
    }
};

#endif
