/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rjasari <rjasari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 20:32:44 by tkruger           #+#    #+#             */
/*   Updated: 2022/03/01 12:18:52 by rjasari          ###   ########.fr       */
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
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include "../libs/libft/includes/libft.h"

/* ************************************************************************** */
/* DEFINES																	  */
/* ************************************************************************** */

# define OPERATORS "|<>"
# define METACHARS OPERATORS"\'\" \t\n"

/* ************************************************************************** */
/* STRUCTS																	  */
/* ************************************************************************** */

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
void	environment_print(void);

/* lexer.c */

void	*lexer(char const *line);

/* syntax_check.c */

bool	syntax_check(t_list *tokens);
bool	is_operator(t_list *token);

/* signal.c */
void	control_c(int sig);
int		change_attributes(bool print_controls);

#endif
