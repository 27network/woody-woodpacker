# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/06 18:05:28 by kiroussa          #+#    #+#              #
#    Updated: 2025/04/06 18:07:38 by kiroussa         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libelfstream.a

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic -std=c11 -g3
CFLAGS += -I./include

LIBFT_DIR = libft
CFLAGS += -I$(LIBFT_DIR)/include

all: $(NAME)

$(NAME): $(OBJ)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
