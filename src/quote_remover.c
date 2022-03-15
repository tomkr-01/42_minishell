/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_remover.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 15:11:04 by tkruger           #+#    #+#             */
/*   Updated: 2022/03/15 14:29:40 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

size_t	next_quote(char *token, char quote)
char	*expansion_char_remover(char *token)

char	*quote_remover(char *token)
{
	char	*q_removed;
	size_t	i;
	char	quote;

	i = 0;
	q_removed = NULL;
	while (token != NULL && token[i] != '\0')
	{
		quote = token[next_quote(token, 0)];
		q_removed = ft_strjoin_free(q_removed,
				ft_substr(token, 0, next_quote(token, 0)));
		token = ft_substr_free(token,
				next_quote(token, 0) + 1, ft_strlen(token));
		q_removed = ft_strjoin_free(q_removed,
				ft_substr(token, 0, next_quote(token, quote)));
		token = ft_substr_free(token,
				next_quote(token, quote) + 1, ft_strlen(token));
	}
	return (expansion_char_remover(q_removed));
}

size_t	next_quote(char	*token, char quote)
{
	size_t	i;
	char	*check_str;

	i = 0;
	while (token != NULL && token[i] != '\0')
	{
		check_str = ft_substr(token, 0, i + 1);
		if (quote == 0 && (token[i] == '\'' || token[i] == '\"')
			&& ft_count_char(check_str, '\31') % 2 == 0)
		{
			free(check_str);
			check_str = NULL;
			return (i);
		}
		else if (quote == token[i] && ft_count_char(check_str, '\31') % 2 == 0)
		{
			free(check_str);
			check_str = NULL;
			return (i);
		}
		i++;
		free(check_str);
	}
	check_str = NULL;
	return (i);
}

char	*expansion_char_remover(char *token)
{
	size_t	i;
	size_t	j;
	char	*c_removed;

	i = 0;
	j = 0;
	c_removed = ft_calloc(ft_strlen(token), sizeof(*c_removed));
	if (c_removed == NULL)
		exit(put_stderr(SHELL, "expansion_char_remover()",
				NULL, strerror(ENOMEM)));
	while (token[i] != '\0')
	{
		if (token[i] == '\31')
			i++;
		else
			c_removed[j++] = token[i++];
	}
	c_removed[j] = '\0';
	return (c_removed);
}
