/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 17:04:32 by tkruger           #+#    #+#             */
/*   Updated: 2022/03/15 14:26:21 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

extern t_minishell	g_msh;

int	valid_exp_char(int c, bool first_char)
{
	if (ft_isalnum(c) || c == '_'
		|| (first_char == true && (c == '?' || c == '\'' || c == '"')))
		return (1);
	return (0);
}

size_t	next_exp(char *token)
{
	size_t	pos;

	pos = 0;
	while (token != NULL && token[pos] != '\0')
	{
		if (token[pos] == '\'')
		{
			pos += ft_strchr_int(&token[pos + 1], '\'') + 1;
		}
		else if (token[pos] == '"')
		{
			while (token[++pos] != '\0' && token[pos] != '"')
			{
				if (token[pos] == '$' && valid_exp_char(token[pos + 1], true))
					return (pos);
			}
		}
		else if (token[pos] == '$' && valid_exp_char(token[pos + 1], true))
		{
			return (pos);
		}
		if (token[pos] != '\0')
			pos++;
	}
	return (pos);
}

char	*get_varname(char *token)
{
	size_t	i;

	i = 1;
	if (token[0] == '\0')
		return (NULL);
	else if (token[1] == '?')
		return (ft_strdup("?"));
	else if (token[1] == '\'' || token[1] == '"')
		return (ft_substr(token, 1, ft_strchr_int(token + 2, token[1]) + 2));
	while (valid_exp_char(token[i], false))
	{
		i++;
	}
	return (ft_substr(token, 1, i - 1));
}

char	*expand_varname(char *varname)
{
	char	*value;

	if (varname == NULL)
		return (NULL);
	else if (strcmp(varname, "?") == 0)
		value = ft_itoa(g_msh.exit_code);
	else if (varname[0] == '\'' || varname[0] == '\"')
		value = ft_strdup(varname);
	else
	{
		value = ft_strjoin_free(ft_strdup("\31"),
				get_var((const char *)varname));
		value = ft_strjoin_free(value, ft_strdup("\31"));
	}
	free(varname);
	return (value);
}

char	*expander(char *token)
{
	char	*expanded;
	size_t	i;

	expanded = NULL;
	i = 0;
	while (token != NULL && token[i] != '\0')
	{
		expanded = ft_strjoin_free(expanded,
				ft_substr(token, 0, next_exp(token)));
		token = ft_substr_free(token,
				next_exp(token), ft_strlen(token));
		expanded = ft_strjoin_free(expanded,
				expand_varname(get_varname(token)));
		token = ft_substr_free(token,
				ft_strlen_free(get_varname(token)) + 1, ft_strlen(token));
	}
	return (quote_remover(expanded));
}
