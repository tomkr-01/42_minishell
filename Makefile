NAME =			minishell
PREP =			obj

CC =			gcc
RM =			rm -rf
CFLAGS =		-g #-Wall -Werror -Wextra
INC =			-I/inc

LIBFT_DIR =		libs/libft
LIBFT =			-L./$(LIBFT_DIR)/ -lft
ARCHIVE =		libs/libft/libft.a
READLINE =		-lreadline

SRC_DIR =		./src/
SRCS =			builtin_cd.c builtin_echo.c builtin_env.c \
				builtin_exit.c builtin_export.c builtin_pwd.c \
				builtin_unset.c builtins.c environment.c \
				executor.c expander.c lexer.c main.c \
				quote_remover.c r_utils.c signal.c \
				syntax_check.c t_utils.c table.c expander_utils.c

OBJ_DIR =		./obj/
OBJS =			$(addprefix $(OBJ_DIR), $(SRCS:.c=.o))

all: $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(NAME): $(PREP) libft_dir $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(READLINE) $(LIBFT)

$(PREP):
	mkdir -p $(OBJ_DIR)

libft_dir:
	make -C $(LIBFT_DIR)

clean:
	make fclean -C ./libs/libft
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all
