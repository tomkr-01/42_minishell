# NAME	=	minishell
# CC		=	gcc
# CFLAGS	+=	-Wall -Wextra -Werror
# INC		=	-I./inc
# SRC_PATH =	./src/
# PREP	=	objs

# SRCS 	=	builtin_cd.c builtin_echo.c builtin_env.c builtin_exit.c \
# 			builtin_export.c builtin_pwd.c builtin_unset.c builtins.c \
# 			environment.c \
# 			exec_child.c exec_pipeline.c exec_redir.c exec_scmd.c \
# 			exec_utils.c exec_wait.c executor.c \
# 			expander_utils.c expander.c \
# 			lexer.c main.c minishell_gnl.c \
# 			parse_heredoc.c parse_redir.c parser.c \
# 			quote_remover.c signal.c syntax_check.c \
# 			utils_array.c utils_clear.c utils_error.c

# OBJ_PATH =	./objs/
# OBJS	=	$(patsubst %c,$(OBJ_PATH)%o,$(SRCS))
# LIBFT	=	-L./libs/libft -lft
# LIBFT_PATH = libs/libft/libft.a
# # iMac
# # READLINE2 =	-I/Users/$(USER)/.brew/opt/readline/include
# READLINE =	-L/Users/$(USER)/.brew/opt/readline/lib -lreadline
# # Macbook
# # READLINE	=	-L/opt/homebrew/opt/readline/lib -lreadline
# # READLINE2	=	-I/opt/homebrew/opt/readline/include
# LIBS	=	$(LIBFT) $(READLINE)

# .PHONY: all $(NAME) $(OBJ_PATH) $(PREP) $(LIBFT_PATH) clean fclean re

# all: $(LIBFT_PATH) $(NAME)

# $(NAME): $(PREP) $(OBJS)
# 	$(CC) $(CFLAGS) $(LIBFT) $(READLINE) $(OBJS) -o $(NAME)

# $(OBJ_PATH)%.o: $(SRC_PATH)%.c $(INC)
# 	$(CC) $(CFLAGS) $(LIBFT) -c $< -o $@

# $(PREP):
# 	mkdir -p $(OBJ_PATH)

# $(LIBFT_PATH):
# 	make -C libs/libft/

# clean:
# 	rm -rf $(OBJ_PATH)
# 	rm -f *.o *~
# 	rm -rf *.dSYM
# 	make clean -C libs/libft/

# fclean: clean
# 	rm -rf $(NAME)
# 	rm -f libs/libft/libft.a

# re: fclean all


NAME = minishell
LIB = -Llib -lft -L$(HOME)/.brew/opt/readline/lib -lreadline
FLAGS = -Wall -Wextra -Werror

INC = -Iinc -I$(HOME)/.brew/opt/readline/include
SRC = 		builtin_cd.c builtin_echo.c builtin_env.c builtin_exit.c \
 			builtin_export.c builtin_pwd.c builtin_unset.c builtins.c \
 			environment.c \
 			exec_child.c exec_pipeline.c exec_redir.c exec_scmd.c \
 			exec_utils.c exec_wait.c executor.c \
 			expander_utils.c expander.c \
 			lexer.c main.c minishell_gnl.c \
 			parse_heredoc.c parse_redir.c parser.c \
 			quote_remover.c signal.c syntax_check.c \
 			utils_array.c utils_clear.c utils_error.c

OBJ_DIR = obj/

OBJ = $(addprefix $(OBJ_DIR), $(patsubst %.c,%.o,$(SRC)))

$(NAME) : libs/libft/libft.a $(OBJ)
	@gcc $(FLAGS) -O3 $(INC) $^ $(LIB) -o $@

	@echo "compiled!"

all : libs/libft/libft.a $(NAME)

$(OBJ_DIR)%.o: %.c
	@gcc $(FLAGS) $(INC) -O3 -c $< -o $@

re : fclean all

bonus : all

libs/libft/libft.a :
	@make -silent -C libs/libft
	@cp -f libs/libft/libft.a lib
	@cp -f libs/libft/includes/libft.h inc/
	@echo "libraries ready!"

clean :
	make clean -silent -C libs/libft/
	- rm -f $(OBJ)
	@echo "object directory cleaned!"

fclean :
	make fclean -silent -C libs/libft/
	rm -f $(OBJ)
	rm -f ./libs/libft/libft.a inc/libft.h
	rm -f ./minishell
	@echo "fully cleaned!"

help :
	@echo "rules:	all [default]"
	@echo "	re"
	@echo "	clean"
	@echo "	fclean"
	@echo "	bonus"

-include $(DEP)

.PHONY : bonus fclean re all clean