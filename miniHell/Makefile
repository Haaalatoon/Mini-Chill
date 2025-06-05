NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f

# Directories
LIBFT_DIR = libft

# Source files
SRCS = minishell.c \
       token.c \
       tokenizer_init.c \
       tokenizer_states1.c \
       tokenizer_states2.c \
       utils.c

# Object files
OBJS = $(SRCS:.c=.o)

# Libraries
LIBFT = $(LIBFT_DIR)/libft.a
LIBS = -L$(LIBFT_DIR) -lft -lreadline

# Include paths
INCLUDES = -I.

# Colors
GREEN = \033[0;32m
RESET = \033[0m

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)
	@echo "$(GREEN)$(NAME) has been compiled!$(RESET)"

$(LIBFT):
	@make -C $(LIBFT_DIR)

%.o: %.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@$(RM) $(OBJS)
	@make clean -C $(LIBFT_DIR)
	@echo "$(GREEN)Object files cleaned!$(RESET)"

fclean: clean
	@$(RM) $(NAME)
	@make fclean -C $(LIBFT_DIR)
	@echo "$(GREEN)All files cleaned!$(RESET)"

re: fclean all

.PHONY: all clean fclean re