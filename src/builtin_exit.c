/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 14:10:26 by tkruger           #+#    #+#             */
/*   Updated: 2022/03/14 14:10:41 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
