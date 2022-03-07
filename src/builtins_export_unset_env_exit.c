/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export_unset_env_exit.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 11:51:30 by tkruger           #+#    #+#             */
/*   Updated: 2022/03/07 20:07:33 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

extern t_minishell	g_msh;

int	export_builtin(char **arguments)
{
	size_t	a_i;
	size_t	i;

	a_i = 0;
	while (arguments[a_i] != NULL)
	{
		if (ft_strchr(arguments[a_i], '=') == NULL)
		{
			a_i++;
			continue ;
		}
		i = 0;
		while (g_msh.env[i] != NULL && ft_strncmp(g_msh.env[i], arguments[a_i],
				ft_strchr_int(arguments[a_i], '=')) != 0)
			i++;
		if (g_msh.env[i] != NULL)
		{
			free(g_msh.env[i]);
			g_msh.env[i] = ft_strdup(arguments[a_i]);
		}
		else
			g_msh.env = add_array_element(g_msh.env, arguments[a_i]);
		a_i++;
	}
	return (EXIT_SUCCESS);
}

int	unset_builtin(char **arguments)
{
	size_t	a_i;
	size_t	i;

	a_i = 0;
	while (arguments[a_i] != NULL)
	{
		i = 0;
		while (g_msh.env[i] != NULL)
		{
			if (ft_strncmp(g_msh.env[i], arguments[a_i],
					ft_strchr_int(g_msh.env[i], '=') - 1) == 0)
			{
				g_msh.env = rm_array_element(g_msh.env, g_msh.env[i]);
				break ;
			}
			i++;
		}
		a_i++;
	}
	return (EXIT_SUCCESS);
}

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

int	exit_builtin(char **arguments)
{
	ft_putendl_fd("exit", STDERR_FILENO);
	if (ft_arrlen(arguments) > 0 && !ft_isint(arguments[0]))
	{
		put_stderr(SHELL, "exit", arguments[0], "numeric argument required");
		exit(255);
	}
	else if (ft_arrlen(arguments) > 1)
	{
		put_stderr(SHELL, "exit", NULL, "too many arguments");
		return (EPERM);
	}
	else if (ft_arrlen(arguments) == 1)
	{
		exit(ft_atoi(arguments[0]));
	}
	else
	{
		exit(EXIT_SUCCESS);
	}
	return (EXIT_SUCCESS);
}
