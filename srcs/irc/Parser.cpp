/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ateca <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 16:40:00 by ateca             #+#    #+#             */
/*   Updated: 2026/04/11 16:40:00 by ateca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/irc/Parser.hpp"
#include <sstream>
#include <iostream>

// Faz o parsing de uma linha bruta: [:prefixo] COMANDO [params] [:trailing]
IRCMessage Parser::parse(const std::string &rawMessage)
{
    IRCMessage msg;
    std::string line = rawMessage;

    // Remove finais de linha padrão (\r\n ou \n)
    if (line.size() >= 2 && line.substr(line.size() - 2) == "\r\n")
        line = line.substr(0, line.size() - 2);
    else if (line.size() >= 1 && line.substr(line.size() - 1) == "\n")
        line = line.substr(0, line.size() - 1);

    if (line.empty())
        return msg;

    size_t pos = 0;

    // Extrai prefixo se começar com ':'
    if (line[0] == ':')
    {
        pos = line.find(' ');
        if (pos == std::string::npos)
            return msg;
        msg.prefix = line.substr(1, pos - 1);
        pos++;
    }

    // Extrai comando
    size_t nextSpace = line.find(' ', pos);
    if (nextSpace == std::string::npos)
    {
        msg.command = line.substr(pos);
        return msg;
    }
    msg.command = line.substr(pos, nextSpace - pos);
    pos = nextSpace + 1;

    // Extrai parâmetros. O ':' indica o parâmetro final que aceita espaços.
    while (pos < line.size())
    {
        while (pos < line.size() && line[pos] == ' ')
            pos++;
        
        if (pos >= line.size())
            break;

        if (line[pos] == ':')
        {
            msg.params.push_back(line.substr(pos + 1));
            break;
        }

        nextSpace = line.find(' ', pos);
        if (nextSpace == std::string::npos)
        {
            msg.params.push_back(line.substr(pos));
            break;
        }
        msg.params.push_back(line.substr(pos, nextSpace - pos));
        pos = nextSpace + 1;
    }

    return msg;
}
