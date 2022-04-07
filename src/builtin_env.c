/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/01 14:24:32 by tkruger           #+#    #+#             */
/*   Updated: 2022/04/01 14:24:33 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

extern t_minishell	g_msh;

int	env_builtin(char **arguments)
{
	size_t	i;

	i = 0;
	if (arguments != NULL && arguments[0] != NULL)
	{
		put_stderr(NULL, "env", arguments[0], strerror(ENOENT));
		return (127);
	}
	while (g_msh.env[i] != NULL)
	{
		ft_putendl_fd(g_msh.env[i], STDOUT_FILENO);
		i++;
	}
	return (EXIT_SUCCESS);
}
