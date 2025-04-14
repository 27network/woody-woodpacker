# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kiroussa <oss@xtrm.me>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/06 18:05:28 by kiroussa          #+#    #+#              #
#    Updated: 2025/04/14 13:12:32 by kiroussa         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libelfstream.a

SRC_DIR = src
OBJ_DIR = obj

ifeq ($(DEVELOPMENT),1)
CFLAGS += -g3 -gdwarf-3 -ggdb -DELFSTREAM_DEBUG=1
_ := $(shell bash gensources.sh $(SRC_DIR))
endif

include sources.mk
OBJS := $(SRCS:.c=.o)
SRCS := $(addprefix $(SRC_DIR)/,$(SRCS))
OBJS := $(addprefix $(OBJ_DIR)/,$(OBJS))

CC = clang
CFLAGS += -Wall -Wextra -Werror -Wpedantic -std=c11
CFLAGS += -I./include

LIBFT_DIR = libft
CFLAGS += -I$(LIBFT_DIR)/include

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
