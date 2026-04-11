/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCMessage.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ateca <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 16:30:00 by ateca             #+#    #+#             */
/*   Updated: 2026/04/11 16:30:00 by ateca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_MESSAGE_HPP
#define IRC_MESSAGE_HPP

#include <string>
#include <vector>

struct IRCMessage
{
    std::string prefix;
    std::string command;
    std::vector<std::string> params;

    bool isValid() const { return !command.empty(); }
};

#endif
