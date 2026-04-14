/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandDispatcher.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ateca <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 17:25:00 by ateca             #+#    #+#             */
/*   Updated: 2026/04/14 23:35:18 by ateca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_DISPATCHER_HPP
#define COMMAND_DISPATCHER_HPP

#include "IRCMessage.hpp"
#include "UserRegistry.hpp"
#include "ChannelManager.hpp"
#include "MessageRouter.hpp"

class CommandDispatcher
{
private:
    UserRegistry &userRegistry;
    ChannelManager &channelManager;
    MessageRouter &messageRouter;

public:
    CommandDispatcher(UserRegistry &ur, ChannelManager &cm, MessageRouter &mr);

    void dispatch(int fd, const IRCMessage &msg);

private:
    void handleNick(User* user, const IRCMessage &msg);
    void handleUser(User* user, const IRCMessage &msg);
    void handlePass(User* user, const IRCMessage &msg);
    void handleJoin(User* user, const IRCMessage &msg);
    void handlePrivmsg(User* user, const IRCMessage &msg);
    void handleQuit(User* user, const IRCMessage &msg);
    void handleStructuredMessage(User* user, const std::string &target, const std::string &json);

    
    void sendWelcome(User* user);
};

#endif
