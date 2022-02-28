/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 10:56:26 by tkruger           #+#    #+#             */
/*   Updated: 2022/02/28 11:41:18 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include "../libs/libft/includes/libft.h"

size_t	quote_token(const char *line, int *i);
size_t	alphanum_token(const char *line, int *i);
size_t	operator_token(const char *line, int *i);

void	*lexer(char const *line)
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
			chrs = quote_token(line, &i);
		else if (ft_strchr(OPERATORS, line[i]))
			chrs = operator_token(line, &i);
		else if (!ft_iswhitespace(line[i]) && !ft_strchr(OPERATORS, line[i]))
			chrs = alphanum_token(line, &i);
		if (chrs != 0)
			ft_lstadd_back(&tokens, ft_lstnew(ft_substr(line, i - chrs, chrs)));
		else
			i++;
	}
	return (tokens);
}

size_t	operator_token(const char *line, int *i)
{
	size_t	chrs;

	chrs = 0;
	if (line[*i] == '|')
	{
		*i += 1;
		chrs += 1;
	}
	else if (line[*i] == '<')
	{
		while (line[*i] == '<')
		{
			*i += 1;
			chrs += 1;
		}
	}
	else if (line[*i] == '>')
	{
		while (line[*i] == '>')
		{
			*i += 1;
			chrs += 1;
		}
	}
	return (chrs);
}

size_t	alphanum_token(const char *line, int *i)
{
	size_t	chrs;

	chrs = 0;
	while (!ft_iswhitespace(line[*i]) && !ft_strchr(METACHARS, line[*i]))
	{
		*i += 1;
		chrs += 1;
	}
	return (chrs);
}

size_t	quote_token(const char *line, int *i)
{
	size_t	chrs;
	int		single_quotes;
	int		double_quotes;

	chrs = 0;
	single_quotes = -1;
	double_quotes = -1;
	while (line[*i] != '\0')
	{
		if (line[*i] == '\'')
			single_quotes *= -1;
		if (line[*i] == '"')
			double_quotes *= -1;
		if ((ft_strchr(OPERATORS, line[*i]) || ft_iswhitespace(line[*i]))
			&& single_quotes == -1 && double_quotes == -1)
			break ;
		*i += 1;
		chrs++;
	}
	return (chrs);
}
