/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/01 14:26:13 by tkruger           #+#    #+#             */
/*   Updated: 2022/04/01 16:08:11 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static char	*heredoc_get_next_line(char **limiter)
{
	char	*line;
	char	*here_string;
	char	*lim;

	signal(SIGINT, &heredoc_signals);
	lim = ft_strjoin(*limiter, "\n");
	here_string = NULL;
	while (1)
	{
		line = minishell_get_next_line(STDIN_FILENO);
		if (line == NULL || ft_strcmp(line, lim) == 0)
		{
			ft_free((void **)&line);
			break ;
		}
		here_string = ft_strjoin_free(here_string, line);
		if (here_string == NULL)
			break ;
	}
	ft_free((void **)&lim);
	return (here_string);
}

static char	*heredoc_readline(char **limiter)
{
	char	*line;
	char	*here_string;

	signal(SIGINT, &heredoc_signals);
	here_string = NULL;
	while (1)
	{
		line = readline("> ");
		if (line == NULL || ft_strcmp(line, *limiter) == 0)
		{
			ft_free((void **)&line);
			return (here_string);
		}
		line = ft_strjoin_free(line, ft_strdup("\n"));
		here_string = ft_strjoin_free(here_string, line);
		if (here_string == NULL)
		{
			ft_free((void **)&line);
			return (NULL);
		}
	}
	return (here_string);
}

void	heredoc(char **token_content)
{
	int		fd;
	bool	expansion;
	char	*limiter;

	change_attributes(false);
	fd = dup(STDIN_FILENO);
	expansion = false;
	limiter = quote_remover(ft_strdup(*token_content));
	if (ft_strcmp(*token_content, limiter) == 0)
		expansion = true;
	ft_free((void **)token_content);
	if (isatty(STDIN_FILENO))
		*token_content = heredoc_readline(&limiter);
	else
		*token_content = heredoc_get_next_line(&limiter);
	ft_free((void **)&limiter);
	if (close(STDIN_FILENO) == -1)
		ft_free((void **)token_content);
	if (dup2(fd, STDIN_FILENO) == -1)
		close(fd);
	close(fd);
	if (expansion)
		*token_content = expander(*token_content, false);
}
