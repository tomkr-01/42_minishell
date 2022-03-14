# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/21 20:28:04 by tkruger           #+#    #+#              #
#    Updated: 2022/03/14 22:13:13 by tkruger          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	minishell
CC		=	gcc
CFLAGS	=	-Wall -Wextra -Werror # -g -fsanitize=address
INC		=	./inc/minishell.h
SRC_PATH =	./src/

# general
SRCS	=	environment.c t_utils.c

# input
SRCS	+=	lexer.c syntax_check.c expander.c quote_remover.c

# builtins
SRCS	+=	builtins.c builtin_cd.c builtin_echo.c builtin_env.c \
			builtin_exit.c builtin_export.c builtin_pwd.c builtin_unset.c

# toms_function_tester.c
# SRCS	+=	toms_function_tester.c

OBJ_PATH =	./objs/
OBJS	=	$(patsubst %c,$(OBJ_PATH)%o,$(SRCS))
LIBFT	=	-L./libs/libft -lft libs/libft/libft.a
LIBS	=	$(LIBFT)

.PHONY: all $(NAME) $(OBJ_PATH) libmake clean fclean re

all: $(NAME)

$(NAME): $(OBJ_PATH) $(OBJS) libmake
	$(CC) $(CFLAGS) $(LIBS) $(OBJS) -o $(NAME)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c $(INC)
	$(CC) $(FLAGS) -c $< -o $@

$(OBJ_PATH):
	@mkdir -p $(OBJ_PATH)

libmake:
	@make -C libs/libft/

clean:
	@rm -rf $(OBJ_PATH)
	@rm -f *.o *~
	@rm -rf *.dSYM
	@make -C libs/libft/ clean
	@echo "\033[1;32m.o files removed!\033[0m"

fclean: clean
	@rm -f $(NAME)
	@rm -f libs/libft/libft.a
	@echo "\033[1;32mbinary files removed!\033[0m" 

re: fclean all