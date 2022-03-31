#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stddef.h>
# include <signal.h>
# include <termios.h>
# include <stdbool.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <string.h>
# include "../libs/libft/includes/libft.h"

/* ************************************************************************** */
/* DEFINES																	  */
/* ************************************************************************** */

# define SHELL "beardshell"
# define PROMPT "beardshell> "
# define OPERATORS "|<>"
# define METACHARS "|<> \t\n"

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

typedef struct s_quotes
{
	int	s;
	int	d;
}	t_quotes;

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
char	*expander(char *token, bool unquote);

/* quote_remover.c */
char	*quote_remover(char *token);

/* syntax_check.c */
bool	syntax_check(t_list *tokens);

/* table.c */
t_table	*parser(t_list *token);
void	redi_clear(t_redirection **redi);

/* r_utils.c */
char	**array_append_array(char **first, char **second);
void	table_clear(t_table **table);

/* t_utils.c */
char	**add_array_element(char **old_arr, char *new_el);
char	**rm_array_element(char **old_arr, char	*old_el);
int		put_stderr(char	*s1, char *s2, char *s3, char *message);
void	del_content(void *ptr);

/* signal.c */
void	control_c(int sig);
void	execution_signals(int sig);
int		change_attributes(bool print_controls);
char	*find_executable(char *command, t_table *head);
void	executioner(t_table *table, t_table *head);
int		count(const char *s, char c);
char	*str_append_char(char *string, char c);
char	*expansion(char *token, bool unquote);
void	free_parser(t_table *table);
size_t	next_exp(char *token, size_t pos);
char	*minishell_get_next_line(int fd);
t_table	*get_head(void);

#endif
