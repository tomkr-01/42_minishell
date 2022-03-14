/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 14:10:02 by tkruger           #+#    #+#             */
/*   Updated: 2022/03/14 14:10:12 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

extern t_minishell	g_msh;

int	env_builtin(char **arguments)
{
	size_t	i;

	i = 0;
	while (g_msh.env[i] != NULL)
	{
		ft_putendl_fd(g_msh.env[i], STDOUT_FILENO);
		i++;
	}
	if (arguments[0] == NULL)
		return (EXIT_SUCCESS);
	put_stderr(NULL, "env", arguments[0], strerror(ENOENT));
	return (127);
}
