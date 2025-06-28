# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hrhilane <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/20 17:02:31 by hrhilane          #+#    #+#              #
#    Updated: 2024/11/26 18:39:53 by hrhilane         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror

# Mandatory and Bonus Sources
MANDATORY_SRCS = ft_atoi.c\
	ft_bzero.c\
	ft_calloc.c\
	ft_isalnum.c\
	ft_isalpha.c\
	ft_isascii.c\
	ft_isdigit.c\
	ft_isprint.c\
	ft_itoa.c\
	ft_memchr.c\
	ft_memcmp.c\
	ft_memcpy.c\
	ft_memmove.c\
	ft_memset.c\
	ft_putchar_fd.c\
	ft_putendl_fd.c\
	ft_putnbr_fd.c\
	ft_putstr_fd.c\
	ft_split.c\
	ft_strchr.c\
	ft_strdup.c\
	ft_striteri.c\
	ft_strjoin.c\
	ft_strlcat.c\
	ft_strlcpy.c\
	ft_strlen.c\
	ft_strmapi.c\
	ft_strncmp.c\
	ft_strnstr.c\
	ft_strrchr.c\
	ft_strtrim.c\
	ft_substr.c\
	ft_tolower.c\
	ft_toupper.c\

BONUS_SRCS = ft_lstadd_back_bonus.c\
	ft_lstadd_front_bonus.c\
	ft_lstclear_bonus.c\
	ft_lstdelone_bonus.c\
	ft_lstiter_bonus.c\
	ft_lstlast_bonus.c\
	ft_lstmap_bonus.c\
	ft_lstnew_bonus.c\
	ft_lstsize_bonus.c

# Object files
MANDATORY_OBJS = $(MANDATORY_SRCS:.c=.o)
BONUS_OBJS = $(BONUS_SRCS:.c=.o)

# Targets
NAME = libft.a
BONUS_PRESENT = .bonus_done

# Default target: only builds the mandatory part
all: $(NAME)

# Build the mandatory library
$(NAME): $(MANDATORY_OBJS)
	ar rcs $@ $?

# Bonus target: adds bonus files to the library
bonus: $(BONUS_PRESENT)

$(BONUS_PRESENT): $(BONUS_OBJS) $(NAME)
	ar rcs $(NAME) $(BONUS_OBJS)
	@touch $(BONUS_PRESENT)

# Compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# To avoid redundant rebuilds
.SECONDARY: $(MANDATORY_OBJS) $(BONUS_OBJS)

# Cleaning rules
clean:
	rm -f $(MANDATORY_OBJS) $(BONUS_OBJS)


fclean: clean
	rm -f $(NAME) $(BONUS_PRESENT)

re: fclean all

# Phony targets
.PHONY: all clean fclean re bonus
