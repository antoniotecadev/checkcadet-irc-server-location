# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ateca <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/11 14:09:32 by ateca             #+#    #+#              #
#    Updated: 2026/04/14 16:26:58 by ateca            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

SRCS = srcs/main.cpp \
    	srcs/server/server.cpp \
		srcs/network/ClientConnection.cpp \
		srcs/irc/Parser.cpp \
		srcs/irc/UserRegistry.cpp \
		srcs/irc/ChannelManager.cpp \
		srcs/irc/MessageRouter.cpp \
		srcs/irc/CommandDispatcher.cpp

INCLUDES = includes/server/server.hpp \
		   includes/network/ClientConnection.hpp \
		   includes/irc/IRCMessage.hpp \
		   includes/irc/Parser.hpp \
		   includes/irc/User.hpp \
		   includes/irc/UserRegistry.hpp \
		   includes/irc/Channel.hpp \
		   includes/irc/ChannelManager.hpp \
		   includes/irc/MessageRouter.hpp \
		   includes/irc/CommandDispatcher.hpp

OBJS = $(SRCS:.cpp=.o)

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17

DOCKER_COMPOSE = docker compose -f ./docker-compose.yml -p checkcadet-irc-server-location

# =========================
# BUILD C++
# =========================

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJS): $(INCLUDES)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

# =========================
# DOCKER
# =========================

build:
	$(DOCKER_COMPOSE) up --build -d

up:
	$(DOCKER_COMPOSE) up -d

down:
	$(DOCKER_COMPOSE) down

start:
	$(DOCKER_COMPOSE) start

stop:
	$(DOCKER_COMPOSE) stop

logs:
	$(DOCKER_COMPOSE) logs -f

ps:
	$(DOCKER_COMPOSE) ps

restart:
	$(DOCKER_COMPOSE) restart

# =========================
# FULL CLEAN
# =========================

docker-clean:
	docker system prune -f

# =========================
# PHONY
# =========================

.PHONY: all clean fclean re build up down start stop logs ps restart docker-clean