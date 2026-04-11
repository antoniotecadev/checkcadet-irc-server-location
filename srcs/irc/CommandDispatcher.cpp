/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandDispatcher.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ateca <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 17:30:00 by ateca             #+#    #+#             */
/*   Updated: 2026/04/11 17:30:00 by ateca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/irc/CommandDispatcher.hpp"
#include <iostream>

CommandDispatcher::CommandDispatcher(UserRegistry &ur, ChannelManager &cm, MessageRouter &mr)
    : userRegistry(ur), channelManager(cm), messageRouter(mr) {}

// Direciona a mensagem para o handler do comando
void CommandDispatcher::dispatch(int fd, const IRCMessage &msg)
{
    User* user = userRegistry.getUserByFd(fd);
    if (!user) return;

    std::string cmd = msg.command;
    for (auto &c : cmd) c = toupper(c); // Normaliza comando para maiúscula

    if (cmd == "NICK") handleNick(user, msg);
    else if (cmd == "USER") handleUser(user, msg);
    else if (cmd == "PASS") handlePass(user, msg);
    else if (cmd == "JOIN") handleJoin(user, msg);
    else if (cmd == "PRIVMSG") handlePrivmsg(user, msg);
    else if (cmd == "QUIT") handleQuit(user, msg);
    else { std::cout << "Indisponível: " << cmd << std::endl; }
}

// Trata mudança ou definição de apelido
void CommandDispatcher::handleNick(User* user, const IRCMessage &msg)
{
    if (msg.params.empty()) return;
    
    std::string newNick = msg.params[0];
    if (userRegistry.isNicknameTaken(newNick))
    {
        messageRouter.sendToUser(user, ":server 433 * " + newNick + " :Nickname is already in use");
        return;
    }

    std::string oldNick = user->getNickname();
    userRegistry.updateNickname(user, newNick);
    
    if (!oldNick.empty())
        messageRouter.sendToUser(user, ":" + oldNick + " NICK " + newNick);
    
    // Completa o registro se NICK e USER já foram recebidos
    if (!user->isRegistered() && !user->getUsername().empty())
        sendWelcome(user);
}

// Trata informações básicas da conta (username, realname)
void CommandDispatcher::handleUser(User* user, const IRCMessage &msg)
{
    if (msg.params.size() < 4) return;
    
    user->setUsername(msg.params[0]);
    user->setRealname(msg.params[3]);
    
    if (!user->isRegistered() && !user->getNickname().empty())
        sendWelcome(user);
}

void CommandDispatcher::handlePass(User* user, const IRCMessage &msg)
{
    (void)user; (void)msg;
}

// Trata entrada em canais
void CommandDispatcher::handleJoin(User* user, const IRCMessage &msg)
{
    if (msg.params.empty()) return;
    
    std::string channelName = msg.params[0];
    if (channelName[0] != '#') channelName = "#" + channelName;
    
    Channel* channel = channelManager.getOrCreateChannel(channelName);
    channel->addMember(user);
    
    // Broadcast oficial de entrada no canal
    std::string joinMsg = ":" + user->getNickname() + "!" + user->getUsername() + "@localhost JOIN " + channelName;
    messageRouter.sendToChannel(channel, joinMsg);
    
    // Envia Tópico e Lista de Nomes (padrão RFC)
    messageRouter.sendToUser(user, ":server 332 " + user->getNickname() + " " + channelName + " :" + channel->getTopic());
    
    std::string names = ":server 353 " + user->getNickname() + " = " + channelName + " :";
    for (User* m : channel->getMembers())
        names += m->getNickname() + " ";
    messageRouter.sendToUser(user, names);
    
    messageRouter.sendToUser(user, ":server 366 " + user->getNickname() + " " + channelName + " :End of NAMES list");
}

// Roteia mensagens para usuários ou canais específicos
void CommandDispatcher::handlePrivmsg(User* user, const IRCMessage &msg)
{
    if (msg.params.size() < 2) return;
    
    std::string target = msg.params[0];
    std::string text = msg.params[1];
    std::string fullMsg = ":" + user->getNickname() + " PRIVMSG " + target + " :" + text;

    if (target[0] == '#')
    {
        Channel* channel = channelManager.getChannel(target);
        if (channel)
            messageRouter.sendToChannel(channel, fullMsg, user);
    }
    else
    {
        User* targetUser = userRegistry.getUserByNickname(target);
        if (targetUser)
            messageRouter.sendToUser(targetUser, fullMsg);
    }
}

void CommandDispatcher::handleQuit(User* user, const IRCMessage &msg)
{
    (void)user; (void)msg;
}

// Envia mensagens de Welcome ao finalizar registro (Handshake)
void CommandDispatcher::sendWelcome(User* user)
{
    user->setRegistered(true);
    messageRouter.sendToUser(user, ":server 001 " + user->getNickname() + " :Welcome to the IRC server");
    messageRouter.sendToUser(user, ":server 002 " + user->getNickname() + " :Your host is server, running version 1.0");
}
