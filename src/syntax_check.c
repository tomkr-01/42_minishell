/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rjasari <rjasari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 22:33:15 by tkruger           #+#    #+#             */
/*   Updated: 2022/03/01 11:24:53 by rjasari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <errno.h>

// for list management and error messages
#include "../libs/libft/includes/libft.h"

// for STDERR_FILENO
#include <unistd.h>

void	operator_check(t_list *tokens);
void	quote_check(t_list *tokens);

bool	syntax_check(void *tokens)
{
	operator_check(tokens);
	quote_check(tokens);
	return (true);
}

void	operator_check(t_list *tokens)
{
	bool	prev_tok_redirection;

	prev_tok_redirection = false;
	if (ft_strncmp(tokens->content, "|", 1) == 0
		|| ft_strncmp(ft_lstlast(tokens)->content, "|", 1) == 0
		|| ft_strncmp(ft_lstlast(tokens)->content, "<", 1) == 0
		|| ft_strncmp(ft_lstlast(tokens)->content, ">", 1) == 0)
	{
		exit(ft_putendl_fd("Operator syntax error", STDERR_FILENO));
	}
	while (tokens != NULL)
	{
		if (ft_strncmp(tokens->content, "|", 1) == 0
			|| ft_strncmp(tokens->content, "<", 1) == 0
			|| ft_strncmp(tokens->content, ">", 1) == 0)
		{
			if (ft_strlen(tokens->content) > 2 || prev_tok_redirection == true)
				exit(ft_putendl_fd("Operator syntax error", STDERR_FILENO));
			prev_tok_redirection = true;
		}
		else
			prev_tok_redirection = false;
		tokens = tokens->next;
	}
}

void	quote_check(t_list *tokens)
{
	while (tokens != NULL)
	{
		if (ft_strncmp(tokens->content, "\'", 1) == 0
			&& ft_count_char(tokens->content, '\'') % 2 != 0)
		{
			exit(ft_putendl_fd("Quote syntax error", STDERR_FILENO));
		}
		if (ft_strncmp(tokens->content, "\"", 1) == 0
			&& ft_count_char(tokens->content, '\"') % 2 != 0)
		{
			exit(ft_putendl_fd("Quote syntax error", STDERR_FILENO));
		}
		tokens = tokens->next;
	}
}
