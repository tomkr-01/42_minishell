/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomkrueger <tomkrueger@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/25 14:20:32 by tkruger           #+#    #+#             */
/*   Updated: 2022/03/03 02:02:10 by tomkrueger       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	**g_env;

void	environment_init(char **envp)
{
	size_t	i;

	i = 0;
	g_env = ft_calloc(ft_arrlen(envp) + 1, sizeof(*g_env));
	if (g_env == NULL)
		exit(ft_putendl_fd("environment_init(): malloc error", STDOUT_FILENO));
	while (envp[i] != NULL)
	{
		g_env[i] = ft_strdup(envp[i]);
		i++;
	}
	g_env[i] = NULL;
}

char	*get_var(char *var)
{
	size_t	i;

	i = 0;
	if (var == NULL)
		return (NULL);
	while (g_env[i] != NULL)
	{
		if (ft_strncmp(g_env[i], var, ft_strlen(var)) == 0)
			return (ft_strdup(ft_strchr(g_env[i], '=') + 1));
		i++;
	}
	return (NULL);
}
