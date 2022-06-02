NAME = philo

OBJ_DIR = obj/
SRC_DIR = src/
INC_DIR = include/

SRCS = philo.c
OBJS = $(SRCS:.c=.o)
OBJS_P = $(addprefix $(OBJ_DIR), $(OBJS))

CC = gcc
CFLAGS = -Wall -Werror -Wextra

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
	@echo "Done cleaning archive $(CURDIR)/$(NAME)"

re: fclean
	@$(MAKE)

.PHONY: all clean fclean re