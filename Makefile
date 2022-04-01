NAME	=	minishell
CC		=	gcc
CFLAGS	+=	-Wall -Wextra -Werror
DEP		= ./inc/minishell.h
INC		= -Iinc -I$(HOME)/.brew/opt/readline/include
SRC_PATH =	./src/
PREP	=	objs

SRCS 	=	builtin_cd.c builtin_echo.c builtin_env.c builtin_exit.c \
			builtin_export.c builtin_pwd.c builtin_unset.c builtins.c \
			environment.c \
			exec_child.c exec_pipeline.c exec_redir.c exec_scmd.c \
			exec_utils.c exec_wait.c executor.c \
			expander_utils.c expander.c \
			lexer.c main.c minishell_gnl.c \
			parse_heredoc.c parse_redir.c parser.c \
			quote_remover.c signal.c syntax_check.c \
			utils_array.c utils_clear.c utils_error.c

OBJ_PATH =	objs/
OBJS	=	$(addprefix $(OBJ_PATH), $(SRCS:.c=.o))
LIBFT	=	-L./libs/libft -lft
LIBFT_PATH = libs/libft/libft.a
READLINE =	-L$(HOME)/.brew/opt/readline/lib -lreadline -I$(HOME)/.brew/opt/readline/include
LIBS	=	$(LIBFT) $(READLINE)

.PHONY: all clean fclean re

all: $(LIBFT_PATH) $(NAME)

$(NAME): $(PREP) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBFT) $(READLINE)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c $(DEP)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(PREP):
	mkdir -p $(OBJ_PATH)

$(LIBFT_PATH):
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
