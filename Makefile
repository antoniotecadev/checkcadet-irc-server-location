# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ateca <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/11 14:09:32 by ateca             #+#    #+#              #
#    Updated: 2026/03/21 11:16:21 by ateca            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

SRCS = srcs/main.cpp \
    	srcs/server/server.cpp \
		srcs/network/ClientConnection.cpp

INCLUDES = includes/server/server.hpp \
		   includes/network/ClientConnection.hpp

OBJS = $(SRCS:.cpp=.o)

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJS): $(INCLUDES)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all