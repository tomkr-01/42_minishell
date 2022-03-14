/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 14:08:57 by tkruger           #+#    #+#             */
/*   Updated: 2022/03/14 14:09:08 by tkruger          ###   ########.fr       */
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
				ft_strchr_int(arguments[a_i], '=') + 1) != 0) // changed ft_strchr_int()
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
