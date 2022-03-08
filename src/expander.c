/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 17:04:32 by tkruger           #+#    #+#             */
/*   Updated: 2022/03/08 18:01:29 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

extern t_minishell g_msh;

int	valid_expansion_character(int c)
{
	if (ft_isalnum(c) || c == '_')
		return (1);
	return (0);
}

char	*find_next_expansion(char *token)
{
	while (token != NULL && *token != '\0')
	{
		if (*token == '\'')
		{
			token = ft_strchr(token + 1, '\'');
			if (token == NULL)
				return (NULL);
		}
		else if (*token == '"')
		{
			while (*++token != '\0' && *token != '"')
			{
				if (*token == '$' && valid_expansion_character(*(token + 1)))
					return (token);
			}
		}
		else if (*token == '$' && valid_expansion_character(*(token + 1)))
		{
			return (token);
		}
		if (*token != '\0')
			token++;
	}
	return (NULL);
}

char	*get_varname(char *token)
{
	
}

char	*expand_varname(char *varname)
{
	
}

char	*expander(char *original)
{
	char	*expanded;
	size_t	i;

	i = 0;
	while (original != NULL && original[i] != '\0')
	{
		expanded = ft_strjoin_free(expanded,
				ft_substr(original, 0, find_next_expansion(original)));
		original = ft_substr_free(original,
				find_next_expansion(original), ft_strlen(original));
		expanded = ft_strjoin_free(expanded,
				expand_varname(get_varname(original)));
		original = ft_substr_free(original,
				ft_strlen(get_varname(original)), ft_strlen(original));
	}
	return (expanded);
}
