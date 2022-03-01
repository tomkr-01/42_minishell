/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rjasari <rjasari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 20:32:44 by tkruger           #+#    #+#             */
/*   Updated: 2022/03/01 12:18:00 by rjasari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stddef.h>
# include "../libs/libft/includes/libft.h"

/* ************************************************************************** */
/* DEFINES																	  */
/* ************************************************************************** */

# define OPERATORS "|<>"
# define METACHARS OPERATORS"\'\" \t"

/* ************************************************************************** */
/* STRUCTS																	  */
/* ************************************************************************** */


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

bool	syntax_check(void *tokens);

/* signal.c */
void	control_c(int sig);
int		change_attributes(bool print_controls);

#endif
