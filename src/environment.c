/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/25 14:20:32 by tkruger           #+#    #+#             */
/*   Updated: 2022/03/07 18:38:45 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_minishell	g_msh;

void	environment_init(char **envp)
{
	size_t	i;

	i = 0;
	g_msh.env = ft_calloc((ft_arrlen(envp) + 1), sizeof(*g_msh.env));
	if (g_msh.env == NULL)
		exit(put_stderr(SHELL, "environment_init()", NULL, strerror(ENOMEM)));
	while (envp[i] != NULL)
	{
		g_msh.env[i] = ft_strdup(envp[i]);
		i++;
	}
	g_msh.env[i] = NULL;
}

char	*get_var(char *var)
{
	size_t	i;

	i = 0;
	if (var == NULL)
		return (NULL);
	while (g_msh.env[i] != NULL)
	{
		if (ft_strncmp(g_msh.env[i], var, ft_strlen(var)) == 0)
			return (ft_strdup(ft_strchr(g_msh.env[i], '=') + 1));
		i++;
	}
	return (NULL);
}
