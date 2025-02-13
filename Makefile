# This Makefile was generated by scaffold
# https://codeberg.org/xtrm/scaffold

NAME = woody-woodpacker
MAKE = make --no-print-directory

CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic -O2 -g
LDFLAGS = 

SRC_DIR = src
INC_DIR = include
CFLAGS += -I$(INC_DIR)

BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj

-include development.mk

ifeq ($(DEVELOPMENT),1)
_ := $(shell bash gensources.sh $(SRC_DIR))
endif
include sources.mk

OBJS := $(addprefix $(OBJ_DIR)/,$(SRCS:.c=.o))
SRCS := $(addprefix $(SRC_DIR)/,$(SRCS))

LIB_DIR = third-party
LIBFT_DIR = $(LIB_DIR)/libft
LIBFT = $(LIBFT_DIR)/build/output/libft.a

CLEAN_DEPS += clean_libft
FCLEAN_DEPS += fclean_libft

CFLAGS += -I$(LIBFT_DIR)/include
LDFLAGS += $(LIBFT)

all: $(NAME)

$(LIBFT_DIR):
	git submodule update --init --recursive

$(LIBFT): $(LIBFT_DIR)
	$(MAKE) -C $(LIBFT_DIR) -j$(shell nproc)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

oclean:
	rm -rf $(BUILD_DIR)

clean: $(CLEAN_DEPS) oclean

fclean: $(FCLEAN_DEPS) oclean
	rm -f $(NAME)

clean_libft:
	$(MAKE) -C $(LIBFT_DIR) clean

fclean_libft:
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

remake: oclean all

.PHONY: all clean fclean re $(CLEAN_DEPS) $(FCLEAN_DEPS)
