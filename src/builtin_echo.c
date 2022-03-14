/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 14:06:49 by tkruger           #+#    #+#             */
/*   Updated: 2022/03/14 21:55:27 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	echo_builtin(char **arguments)
{
	size_t	i;
	size_t	j;
	int		new_line;

	i = 0;
	new_line = 1;
	while (arguments != NULL && arguments[i] != NULL)
	{
		j = 1;
		while (arguments[i][0] == '-' && arguments[i][j] == 'n')
			j++;
		if (arguments[i][j] == '\0' && new_line > 0)
			new_line = 2;
		else
		{
			if (new_line > 0)
				new_line *= -1;
			ft_putstr_fd(arguments[i], STDOUT_FILENO);
			ft_putchar_fd(' ', STDOUT_FILENO);
		}
		i++;
	}
	if (new_line == -1)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
