/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ateca <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 16:45:00 by ateca             #+#    #+#             */
/*   Updated: 2026/04/11 16:45:00 by ateca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <unordered_map>

class User
{
private:
    int fd;                 // Socket do cliente
    std::string nickname;   // Apelido (NICK)
    std::string username;   // Conta (USER)
    std::string realname;   // Nome real
    bool authenticated;     // Auth status
    bool registered;        // Handshake status (NICK+USER)

public:
    User(int fd) : fd(fd), authenticated(false), registered(false) {}

    int getFd() const { return fd; }
    const std::string &getNickname() const { return nickname; }
    const std::string &getUsername() const { return username; }
    const std::string &getRealname() const { return realname; }

    void setNickname(const std::string &nick) { nickname = nick; }
    void setUsername(const std::string &user) { username = user; }
    void setRealname(const std::string &real) { realname = real; }
    
    bool isAuthenticated() const { return authenticated; }
    void setAuthenticated(bool auth) { authenticated = auth; }

    bool isRegistered() const { return registered; }
    void setRegistered(bool reg) { registered = reg; }
};

#endif
