/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 10:56:26 by tkruger           #+#    #+#             */
/*   Updated: 2022/02/22 22:11:22 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include "../libs/libft/includes/libft.h"

#include <stdbool.h>
#include <stdio.h>

size_t	next_delimiting_char(const char *line);

t_list	*split_tok(char const *line)
{
	t_list	*tokens;
	int		i;
	size_t	chrs;

	i = 0;
	tokens = NULL;
	while (line != NULL && i < ft_strlen(line))
	{
		chrs = 0;
		if (line[i] == '"' || line[i] == '\'')
		{
			chrs = next_delimiting_char(&line[i]) + 1;
			i += next_delimiting_char(&line[i]);
			printf("i: %i, chrs: %zu\n", i, chrs);

		}
		else if (ft_strchr(OPERATORS, line[i]))
		{
			while (line[i] != '\0' && (line[i] == '<' || line[i] == '>') && line[i] == line[i + 1])
			{
				i++;
				chrs++;
			}
			chrs++;
		}
		else if (!ft_iswhitespace(line[i]) && !ft_strchr(OPERATORS, line[i]))
		{
			while (line[i] != '\0' && !ft_iswhitespace(line[i]) && !ft_strchr(OPERATORS, line[i]))
			{
				i++;
				chrs++;
			}
			i--;
		}
		if (i++ >= 0 && chrs != 0)
			ft_lstadd_back(&tokens, ft_lstnew(ft_substr(line, i - chrs, chrs)));
	}
	return (tokens);
}

size_t	next_delimiting_char(const char *line)
{
	size_t	i;
	int		single_quotes;
	int		double_quotes;

	i = 0;
	single_quotes = -1;
	double_quotes = -1;
	if (line == NULL)
		return (0);
	while (line[i] != '\0')
	{
		if (line[i] == '\'')
			single_quotes *= -1;
		if (line[i] == '"')
			double_quotes *= -1;
		if ((ft_strchr(OPERATORS, line[i]) || ft_iswhitespace(line[i]))
			&& single_quotes == -1 && double_quotes == -1)
			break ;
		i++;
	}
	return (i - 1);
}
