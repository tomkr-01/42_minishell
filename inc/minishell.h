/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomkrueger <tomkrueger@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 20:32:44 by tkruger           #+#    #+#             */
/*   Updated: 2022/03/03 01:13:46 by tomkrueger       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stddef.h>
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

int	main(int argc, char **argv, char **envp);

/* environment.c */

void	environment_init(char **envp);
char	*get_var(char *var);

/* lexer.c */

void	*lexer(char const *line);

/* syntax_check.c */

bool	syntax_check(t_list *tokens);
bool	is_operator(t_list *token);

/* utils.c */

char	**add_array_element(char **old_arr, char *new_el);
char	**rm_array_element(char **old_arr, char	*old_el);
void	free_array(char ***arr);

#endif
