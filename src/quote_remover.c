/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_remover.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 15:11:04 by tkruger           #+#    #+#             */
/*   Updated: 2022/03/14 22:12:59 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

size_t	next_quote(char	*token, char quote)
{
	size_t	i;

	if (token == NULL || token[0] == '\0')
		return (0);
	i = 0;
	while (token[i] != '\0')
	{
		if (quote == 0 && (token[i] == '\'' || token[i] == '\"'))
			return (i);
		else if (quote == token[i])
			return (i);
		i++;
	}
	return (i);
}

char	*quote_remover(char *token)
{
	char	*q_removed;
	size_t	i;
	char	quote;

	i = 0;
	q_removed = NULL;
	quote = 0;
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
	return (q_removed);
}
