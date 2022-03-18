/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 12:43:44 by tkruger           #+#    #+#             */
/*   Updated: 2022/03/18 13:45:16 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

extern t_minishell	g_msh;

int		cd_builtin(char **arguments);
int		echo_builtin(char **arguments);
int		pwd_builtin(char **arguments);
int		export_builtin(char **arguments);
int		unset_builtin(char **arguments);
int		env_builtin(char **arguments);
int		exit_builtin(char **arguments);

bool	check_builtins(char **arguments)
{
	if (strcmp(arguments[0], "cd") == 0
		|| strcmp(arguments[0], "echo") == 0
		|| strcmp(arguments[0], "env") == 0
		|| strcmp(arguments[0], "exit") == 0
		|| strcmp(arguments[0], "export") == 0
		|| strcmp(arguments[0], "unset") == 0
		|| strcmp(arguments[0], "pwd") == 0)
		return (true);
	else
		return (false);
}

void	builtins(char **arguments)
{
	if (strcmp(arguments[0], "cd") == 0)
		g_msh.exit_code = cd_builtin(&arguments[1]);
	else if (strcmp(arguments[0], "echo") == 0)
		g_msh.exit_code = echo_builtin(&arguments[1]);
	else if (strcmp(arguments[0], "env") == 0)
		g_msh.exit_code = env_builtin(&arguments[1]);
	else if (strcmp(arguments[0], "exit") == 0)
		g_msh.exit_code = exit_builtin(&arguments[1]);
	else if (strcmp(arguments[0], "export") == 0)
		g_msh.exit_code = export_builtin(&arguments[1]);
	else if (strcmp(arguments[0], "unset") == 0)
		g_msh.exit_code = unset_builtin(&arguments[1]);
	else if (strcmp(arguments[0], "pwd") == 0)
		g_msh.exit_code = pwd_builtin(&arguments[1]);
}
