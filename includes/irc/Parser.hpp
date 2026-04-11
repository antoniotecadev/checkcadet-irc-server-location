/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ateca <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 16:35:00 by ateca             #+#    #+#             */
/*   Updated: 2026/04/11 16:35:00 by ateca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include "IRCMessage.hpp"
#include <string>

class Parser
{
public:
    static IRCMessage parse(const std::string &rawMessage);
};

#endif
