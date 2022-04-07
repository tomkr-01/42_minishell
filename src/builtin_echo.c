/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/01 14:24:29 by tkruger           #+#    #+#             */
/*   Updated: 2022/04/01 14:24:30 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	echo_builtin(char **arguments)
{
	size_t	i;
	size_t	j;
	int		new_line;

	i = -1;
	new_line = 1;
	while (arguments != NULL && arguments[++i] != NULL)
	{
		j = 0;
		while (arguments[i][0] == '-' && arguments[i][++j] == 'n')
			;
		if (arguments[i][j] == '\0' && new_line > 0 && j > 0)
			new_line = 2;
		else
		{
			if (new_line > 0)
				new_line *= -1;
			ft_putstr_fd(arguments[i], STDOUT_FILENO);
			if (arguments[i + 1] != NULL)
				ft_putchar_fd(' ', STDOUT_FILENO);
		}
	}
	if (-2 < new_line && new_line < 2)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
