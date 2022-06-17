# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: lsinke <lsinke@student.codam.nl>             +#+                      #
#                                                    +#+                       #
#    Created: 2022/06/16 17:24:45 by lsinke        #+#    #+#                  #
#    Updated: 2022/06/16 17:24:45 by lsinke        ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME = philo_bonus

OBJ_DIR = obj/
SRC_DIR = src/
INC_DIR = include/

SRCS =	philo.c														\
		parse.c														\
		init.c														\
		utils.c														\
		sim.c														\
		philosopher.c												\
		philo_action.c												\
		message.c

OBJS = $(SRCS:.c=.o)
OBJS_P = $(addprefix $(OBJ_DIR), $(OBJS))

CC = gcc
CFLAGS += -Wall -Werror -Wextra -g -pthread

ifeq ($(TSAN), 1)
CFLAGS += -fsanitize=thread
endif
ifeq ($(DEBUG), 1)
CFLAGS += -DDEBUG
endif

all: $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(INC_DIR)philo.h
	@mkdir -p $(@D)
	@echo "Compiling: $<"
	@$(CC) $(CFLAGS) -I $(INC_DIR) -c -o $@ $<

$(NAME): $(OBJS_P)
	@echo "Compiling main executable"
	@$(CC) $(CFLAGS) $(OBJS_P) -o $(NAME)

clean:
	@rm -rf $(OBJ_DIR)
	@echo "Done cleaning $(CURDIR)/$(OBJ_DIR)"

fclean: clean
	@rm -f $(NAME)
	@echo "Done cleaning executable $(CURDIR)/$(NAME)"

re: fclean
	@$(MAKE)

.PHONY: all clean fclean re