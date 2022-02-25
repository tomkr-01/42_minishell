/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/25 14:20:32 by tkruger           #+#    #+#             */
/*   Updated: 2022/02/25 14:24:19 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char **g_env;

void	environment_init(char **envp)
{
	int	i;

	i = 0;
	g_env = ft_calloc(ft_arrlen(envp) + 1, sizeof(*g_env));
	if (g_env == NULL)
		exit(ft_putendl_fd("environment_init(): env ft_calloc error", STDOUT_FILENO));
	while (envp[i] != NULL)
	{
		g_env[i] = ft_strdup(envp[i]);
		i++;
	}
	g_env[i] = NULL;
}

void	environment_print(void)
{
	int	i;

	i = 0;
	while (g_env[i] != NULL)
		ft_putendl_fd(g_env[i++], STDOUT_FILENO);
}
