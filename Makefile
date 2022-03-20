NAME =			minishell

CC =			gcc
CFLAGS =		-g #-Wall -Werror -Wextra
INC =			./inc/minishell.h
L_INC =			./libs/libft/includes/libft.h
SRC_PATH =		./src/

SRCS =			builtin_cd.c builtin_echo.c \
				builtin_env.c builtin_exit.c \
				builtin_export.c builtin_pwd.c \
				builtin_unset.c builtins.c \
				environment.c executor.c \
				expander.c lexer.c main.c \
				quote_remover.c r_utils.c \
				signal.c syntax_check.c \
				t_utils.c table.c

OBJ_PATH =		./obj/
OBJS =			$(patsubst %c, $(OBJ_PATH)%o,$(SRCS))
LIBFT =			-L./libs/libft -lft libs/libft/libft.a
READLINE =		-lreadline

all: $(NAME)

$(NAME): $(OBJ_PATH) $(OBJS) libmake
	$(CC) $(CFLAGS) $(LIBFT) $(READLINE) $(OBJS) -o $(NAME)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c $(INC) $(L_INC)
	$(CC) $(CFLAGS) $(LIBFT) -c $< -o $@

$(OBJ_PATH):
	mkdir -p $(OBJ_PATH)

libmake:
	make -C libs/libft

clean:
	rm -rf $(OBJ_PATH)
	rm -f *.o *~
	rm -rf *.dSYM
	make -C libs/libft/ clean

fclean:
	rm -rf $(NAME)
	rm -f libs/libft/libft.a

re: fclean all