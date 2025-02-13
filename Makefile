# This Makefile was generated by scaffold
# https://codeberg.org/xtrm/scaffold

NAME = woody_woodpacker
VERSION = 0.1.0

MAKE = make --no-print-directory

CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic
LDFLAGS = 
NASM = nasm
NASMFLAGS = -f elf64

CFLAGS += -DWW_PROJECT_NAME=\"$(NAME)\"
CFLAGS += -DWW_PROJECT_VERSION=\"$(VERSION)\"

SRC_DIR = src
INC_DIR = include
CFLAGS += -I$(INC_DIR)

BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj

-include development.mk

ifeq ($(DEVELOPMENT),1)
CFLAGS += -g3 -gdwarf-3 -ggdb -DWW_DEBUG=1
_ := $(shell bash gensources.sh $(SRC_DIR))
endif
include sources.mk

OBJS := $(addprefix $(OBJ_DIR)/,$(patsubst %.c,%.o,$(patsubst %.s,%.o,$(SRCS))))
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

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.s
	@mkdir -p $(dir $@)
	$(NASM) $(NASMFLAGS) -o $@ $<

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

.PHONY: all oclean clean fclean re remake $(CLEAN_DEPS) $(FCLEAN_DEPS)
