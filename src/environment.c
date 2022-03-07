/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rjasari <rjasari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/25 14:20:32 by tkruger           #+#    #+#             */
/*   Updated: 2022/03/07 14:37:36 by rjasari          ###   ########.fr       */
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
