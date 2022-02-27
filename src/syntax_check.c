/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 22:33:15 by tkruger           #+#    #+#             */
/*   Updated: 2022/02/28 00:12:04 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// for list management and error messages
#include "../libs/libft/includes/libft.h"

// for STDERR_FILENO
#include <unistd.h>

void	pipe_check(t_list *tokens);
void	redirection_check(t_list *tokens);
void	quote_check(t_list *tokens);

bool	syntax_check(void *tokens)
{
	pipe_check(tokens);
	redirection_check(tokens);
	quote_check(tokens);
	return (true);
}

void	pipe_check(t_list *tokens)
{
	if (ft_strncmp(tokens->content, "|", 1) == 0
		|| ft_strncmp(ft_lstlast(tokens)->content, "|", 1) == 0)
	{
		exit(ft_putendl_fd("Pipe syntax error", STDERR_FILENO));
	}
	while (tokens != NULL && tokens->next != NULL)
	{
		if (ft_strncmp(tokens->content, "|", 1) == 0
			&& ft_strncmp(tokens->next->content, "|", 1) == 0)
		{
			exit(ft_putendl_fd("Pipe syntax error", STDERR_FILENO));
		}
		tokens = tokens->next;
	}
}

void	redirection_check(t_list *tokens)
{
	bool	prev_tok_redirection;

	prev_tok_redirection = false;
	while (tokens != NULL)
	{
		if (ft_strncmp(tokens->content, "<", 1) == 0)
		{
			if (ft_strlen(tokens->content) > 3 || prev_tok_redirection == true)
			{
				exit(ft_putendl_fd("Redirection syntax error", STDERR_FILENO));
			}
			prev_tok_redirection = true;
		}
		else if (ft_strncmp(tokens->content, ">", 1) == 0)
		{
			if (ft_strlen(tokens->content) > 2 || prev_tok_redirection == true)
			{
				exit(ft_putendl_fd("Redirection syntax error", STDERR_FILENO));
			}
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
