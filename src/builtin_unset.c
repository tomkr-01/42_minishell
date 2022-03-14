/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 14:09:31 by tkruger           #+#    #+#             */
/*   Updated: 2022/03/14 14:09:41 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

extern t_minishell	g_msh;

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
					ft_strchr_int(g_msh.env[i], '=')) == 0) // changed ft_strchr_int()
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
