/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomkrueger <tomkrueger@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 20:32:44 by tkruger           #+#    #+#             */
/*   Updated: 2022/03/09 02:33:50 by tomkrueger       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stddef.h>
# include <signal.h>
# include <termios.h>
# include <stdbool.h>
# include <stdlib.h>
# include <errno.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include "../libs/libft/includes/libft.h"

/* ************************************************************************** */
/* DEFINES																	  */
/* ************************************************************************** */

# define SHELL "minishell"
# define OPERATORS "|<>"
# define METACHARS OPERATORS" \t\n"

/* ************************************************************************** */
/* STRUCTS																	  */
/* ************************************************************************** */

typedef struct s_minishell
{
	char	**env;
	int		exit_code;
}	t_minishell;

typedef struct s_cmd
{
	char			*executable;
	char			**arguments;
	char			**out_redirs;
	char			**appends;
	char			*in_redir;
	char			*delimiter;
	bool			pipe;
	struct s_cmd	*next;
}	t_cmd;

/* ************************************************************************** */
/* FUNCTION PROTOTYPES														  */
/* ************************************************************************** */

/* minishell.c */

// int	main(int argc, char **argv, char **envp);

/* environment.c */

void	environment_init(char **envp);
char	*get_var(const char *variable);

/* builtins*.c */

int		builtins(char **arguments);

/* lexer.c */

void	*lexer(char const *line);

/* expander.c */

char	*expander(char *token);

/* syntax_check.c */

bool	syntax_check(t_list *tokens);

/* utils.c */

void	set_exit_code(int code);
char	**add_array_element(char **old_arr, char *new_el);
char	**rm_array_element(char **old_arr, char	*old_el);
int		put_stderr(char	*s1, char *s2, char *s3, char *message);

/* signal.c */
void	control_c(int sig);
int		change_attributes(bool print_controls);

#endif
