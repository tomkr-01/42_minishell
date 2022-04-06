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

t_minishell	g_msh;

/* ************************************************************************** */
/* FUNCTION PROTOTYPES														  */
/* ************************************************************************** */

char	*minishell_get_next_line(int fd);
void	environment_init(char **envp);

void	*lexer(char const *line);
bool	syntax_check(t_list *tokens);
t_table	*parser(t_list *token);
void	executioner(t_table *table);

// parser
void	heredoc(char **token_content);
int		append_redirection(t_redirection **redir, t_list *token,
			int redir_type);

// executor
int		own_fork(pid_t *process_id);
void	filestream_operations(int *initial_stdin, int *initial_stdout,
			int mode);
char	*find_executable(char *command);

void	wait_for_last(int pid, int initial_stdin, int initial_stdout);
void	wait_for_all(int pid, int initial_stdin, int initial_stdout);

void	execute_redirections(t_redirection **redirections, int **pipe_ends,
			int *status);

void	execute_child(t_table **table, int *status);
void	child_process(t_table **table, int **pipe_ends, int *pipe_flag);

void	simple_command(t_table *table);
void	execute_pipeline(t_table *table);

// builtins
void	builtins(char **arguments);
bool	check_builtins(char **arguments);

// signals
void	heredoc_signals(int sig);
void	control_c(int sig);
int		change_attributes(bool print_controls);

// utils
t_table	*get_head(void);
void	del_content(void *ptr);
void	table_clear(t_table **table);

int		put_stderr(char	*shell_name, char *cmd, char *arg, char *message);
char	**add_array_element(char **old_arr, char *new_el);
char	**rm_array_element(char **old_arr, char	*old_el);
char	**array_append_array(char **first, char **second);

char	*get_var(const char *variable);
char	*expander(char *token, bool unquote);
char	*quote_remover(char *expanded);

int		valid_exp_char(int c, bool first_char);

void	ft_atexit(int status);

#endif
