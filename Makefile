# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ateca <ateca@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/11 14:09:32 by ateca             #+#    #+#              #
#    Updated: 2026/03/11 19:05:30 by ateca            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

SRCS = srcs/main.cpp \
      srcs/server/server.cpp

# INCLUDES = includes/server/server.hpp

OBJS = $(SRCS:.cpp=.o)

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

# $(OBJS): $(INCLUDES)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all