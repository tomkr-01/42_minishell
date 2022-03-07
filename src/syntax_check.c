/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 22:33:15 by tkruger           #+#    #+#             */
/*   Updated: 2022/03/07 18:58:14 by tkruger          ###   ########.fr       */
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
bool	is_operator(t_list *token);

bool	syntax_check(t_list *tokens)
{
	operator_check(tokens);
	quote_check(tokens);
	return (true);
}

void	operator_check(t_list *tokens)
{
	int	prev_op;

	prev_op = 1;
	if (ft_strncmp(ft_lstlast(tokens)->content, "<", 1) == 0
		|| ft_strncmp(ft_lstlast(tokens)->content, ">", 1) == 0)
		exit(put_stderr(SHELL, NULL, NULL,
				"syntax error near unexpected token"));
	while (tokens != NULL)
	{
		if (is_operator(tokens))
		{
			if (ft_strlen(tokens->content) > 2 || prev_op == 2
				|| (ft_strncmp(tokens->content, "|", 1) == 0 && prev_op == 1))
				exit(put_stderr(SHELL, NULL, NULL,
						"syntax error near unexpected token"));
			prev_op = 2;
			if (ft_strncmp(tokens->content, "|", 1) == 0)
				prev_op = 1;
		}
		else
			prev_op = 0;
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
			exit(put_stderr(SHELL, NULL, NULL, "syntax error near quote"));
		}
		if (ft_strncmp(tokens->content, "\"", 1) == 0
			&& ft_count_char(tokens->content, '\"') % 2 != 0)
		{
			exit(put_stderr(SHELL, NULL, NULL, "syntax error near quote"));
		}
		tokens = tokens->next;
	}
}

bool	is_operator(t_list *token)
{
	if (ft_strncmp(token->content, "|", 1) == 0
		|| ft_strncmp(token->content, "<", 1) == 0
		|| ft_strncmp(token->content, ">", 1) == 0)
	{
		return (true);
	}
	return (false);
}
