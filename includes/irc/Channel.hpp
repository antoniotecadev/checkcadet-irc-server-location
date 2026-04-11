/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ateca <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 17:00:00 by ateca             #+#    #+#             */
/*   Updated: 2026/04/11 17:00:00 by ateca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "User.hpp"
#include <string>
#include <vector>
#include <algorithm>

class Channel
{
private:
    std::string name;          // Ex: #geral
    std::string topic;         // Tópico do canal
    std::vector<User*> members; // Usuários presentes
    std::vector<User*> operators; // Operadores (@)

public:
    Channel(const std::string &name) : name(name) {}

    const std::string &getName() const { return name; }
    const std::string &getTopic() const { return topic; }
    void setTopic(const std::string &t) { topic = t; }

    void addMember(User* user)
    {
        if (std::find(members.begin(), members.end(), user) == members.end())
        {
            members.push_back(user);
        }
    }

    void removeMember(User* user)
    {
        members.erase(std::remove(members.begin(), members.end(), user), members.end());
        operators.erase(std::remove(operators.begin(), operators.end(), user), operators.end());
    }

    const std::vector<User*> &getMembers() const { return members; }

    bool isOperator(User* user) const
    {
        return std::find(operators.begin(), operators.end(), user) != operators.end();
    }

    void addOperator(User* user)
    {
        if (std::find(operators.begin(), operators.end(), user) == operators.end())
        {
            operators.push_back(user);
        }
    }
    
    bool isEmpty() const { return members.empty(); }
};

#endif
