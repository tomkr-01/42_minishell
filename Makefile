NAME	=	minishell
CC		=	gcc
# CFLAGS	=	-Wall -Wextra -Werror
CFLAGS	+=	-g -fsanitize=address
INC		=	./inc/minishell.h
SRC_PATH =	./src/

# general
SRCS	=	main.c signal.c environment.c t_utils.c r_utils.c

# input
SRCS	+=	lexer.c syntax_check.c expander.c quote_remover.c

# parser
SRCS	+=	table.c

# executor
SRCS	+= executor.c

# builtins
SRCS	+=	builtins.c builtin_cd.c builtin_echo.c builtin_env.c \
			builtin_exit.c builtin_export.c builtin_pwd.c builtin_unset.c

OBJ_PATH =	./objs/
OBJS	=	$(patsubst %c,$(OBJ_PATH)%o,$(SRCS))
LIBFT	=	-L./libs/libft libs/libft/libft.a
# iMac
# READLINE2 =	-I/Users/$(USER)/.brew/opt/readline/include
# READLINE =	-L/Users/$(USER)/.brew/opt/readline/lib -lreadline
# Macbook
READLINE	=	-L/opt/homebrew/opt/readline/lib -lreadline
READLINE2	=	-I/opt/homebrew/opt/readline/include
LIBS	=	$(LIBFT) $(READLINE) $(READLINE2)

.PHONY: all $(NAME) $(OBJ_PATH) libmake clean fclean re

all: libmake $(NAME)

$(NAME): $(OBJ_PATH) $(OBJS)
	$(CC) $(CFLAGS) $(LIBFT) $(READLINE) $(OBJS) -o $(NAME)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c $(INC) $(L_INC)
	$(CC) $(CFLAGS) $(LIBFT) -c $< -o $@

$(OBJ_PATH):
	mkdir -p $(OBJ_PATH)

libmake:
	make -C libs/libft/

clean:
	rm -rf $(OBJ_PATH)
	rm -f *.o *~
	rm -rf *.dSYM
	make clean -C libs/libft/

fclean: clean
	rm -rf $(NAME)
	rm -f libs/libft/libft.a

re: fclean all