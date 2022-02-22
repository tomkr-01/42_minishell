/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 10:56:26 by tkruger           #+#    #+#             */
/*   Updated: 2022/02/22 16:47:20 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include "../libs/libft/includes/libft.h"

#include <stdbool.h>
#include <stdio.h>

void	*split_tok(char const *line)
{
	t_list	*tokens;
	char	*token;
	int		i;
	int		chrs;

	i = 0;
	chrs = 0;
	tokens = NULL;
	if (line == NULL)
		return (NULL);
	while (i < ft_strlen(line))
	{
		chrs = 0;
		while (ft_iswhitespace(line[i]))
			i++;
		if (line[i] == '"')
		{
			token = ft_substr(line, i + 1, ft_strchr_int(&line[i + 1], '"') - 1);
			i += ft_strchr_int(&line[i + 1], '"');
		}
		else if (line[i] == '\'')
		{
			token = ft_substr(line, i + 1, ft_strchr_int(&line[i + 1], '\'') - 1);
			i += ft_strchr_int(&line[i + 1], '\'');
		}
		else if (ft_strchr(OPERATORS, line[i]))
		{
			while (line[i] != '\0' && (line[i] == '<' || line[i] == '>') && line[i] == line[i + 1])
			{
				i++;
				chrs++;
			}
			token = ft_substr(line, i - chrs, chrs + 1);
		}
		else if (!ft_iswhitespace(line[i]) && !ft_strchr(OPERATORS, line[i]))
		{
			while (line[i] != '\0' && !ft_iswhitespace(line[i]) && !ft_strchr(OPERATORS, line[i]))
			{
				i++;
				chrs++;
			}
			token = ft_substr(line, i - chrs, chrs);
		}
		i++;
		ft_lstadd_back(&tokens, ft_lstnew((void *)token));
	}
	return (tokens);
}

size_t	count_delimiters(char *line)
{
	size_t	count;
	size_t	i;
	int		flag;

	count = 0;
	i = 0;
	flag = 0;
	if (line == NULL)
		return 0;
	while (line[i] != '\0')
	{
		if ((line[i] == '"' || line[i] == '\'') && flag >= 0)
		{
			flag = -1;
			printf("%*c\n", i + 1, 'A');
			count++;
		}
		else if ((line[i] == '"' || line[i] == '\'') && flag == -1)
		{
			flag = 0;
		}
		else if ((ft_iswhitespace(line[i]) || ft_strchr(OPERATORS, line[i])) && flag >= 0)
		{
			if (!ft_iswhitespace(line[i]) && (line[i] != line[i + 1] || line[i] == '|'))
			{
				printf("%*c\n", i + 1, 'A');
				count++;
			}
			flag = 0;
		}
		else if (!ft_iswhitespace(line[i]) && !ft_strchr(OPERATORS, line[i]))
		{
			if (flag == 0)
			{
				printf("%*c\n", i + 1, 'A');
				count++;
			}
			if (flag >= 0)
				flag = 1;
		}
		i++;
	}
	printf("\n");
	return (count);
}
