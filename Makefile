# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lrosa-do <lrosa-do@student.42lisboa>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/20 18:22:02 by lrosa-do          #+#    #+#              #
#    Updated: 2023/02/12 19:29:23 by lrosa-do         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =		main

CXX =		g++
CFLAGS =	 -Wall  -lSDL2 -lGL -lm
CFLAGS += -g -fsanitize=address

SRCS =		main.cpp glad.cpp stb_image.cpp

OBJS =		$(SRCS:%.cpp=%.o)

all: $(NAME)

clean:
			rm -rf  $(OBJS) 

fclean:		clean
			rm   $(NAME)

run:
	@./$(NAME) 
	
	

re:			fclean $(NAME)

.PHONY:		all clean fclean re

$(NAME):	$(SRCS)
	$(CXX)  $(SRCS)  $(CFLAGS) -o $(NAME)
	@./$(NAME) 
