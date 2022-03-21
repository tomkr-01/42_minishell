#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stddef.h>
# include <signal.h>
# include <termios.h>
# include <stdbool.h>
# include <stdlib.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include "../libs/libft/includes/libft.h"

/* ************************************************************************** */
/* DEFINES																	  */
/* ************************************************************************** */

# define SHELL "minishell"
# define OPERATORS "|<>"
# define METACHARS OPERATORS" \t\n"

# define PIPE "|"
# define OUT 1
# define IN 2
# define APPEND 4
# define HEREDOC 8

# define READ 0
# define WRITE 1

/* ************************************************************************** */
/* STRUCTS																	  */
/* ************************************************************************** */

typedef struct s_minishell
{
	char	**env;
	int		exit_code;
}	t_minishell;

typedef struct s_redirection {
	int						type;
	char					*name;
	struct s_redirection	*next;
}			t_redirection;

typedef struct s_table {
	char					**arguments;
	t_redirection			*redirections;
	struct s_table			*next;
}			t_table;

/* ************************************************************************** */
/* FUNCTION PROTOTYPES														  */
/* ************************************************************************** */

/* environment.c */
void	environment_init(char **envp);
char	*get_var(const char *variable);

/* builtins*.c */
bool	check_builtins(char **arguments);
void	builtins(char **arguments);

/* lexer.c */
void	*lexer(char const *line);

/* expander.c */
int		valid_exp_char(int c, bool first_char);
char	*expander(char *token);

/* quote_remover.c */
char	*quote_remover(char *token);

/* syntax_check.c */
bool	syntax_check(t_list *tokens);

/* table.c */
t_table	*parser(t_list *token);
void	executioner(t_table *table);

/* r_utils.c */
char	**array_append_array(char **first, char **second);

/* t_utils.c */
char	**add_array_element(char **old_arr, char *new_el);
char	**rm_array_element(char **old_arr, char	*old_el);
int		put_stderr(char	*s1, char *s2, char *s3, char *message);
void	del_content(void *ptr);

/* signal.c */
void	control_c(int sig);
void	execution_signals(int sig);
int		change_attributes(bool print_controls);
char	*find_executable(char *command);
void	executioner(t_table *table);
int		count(const char *s, char c);
void	ft_free(void **ptr);

#endif
