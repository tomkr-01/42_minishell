/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/01 14:25:10 by tkruger           #+#    #+#             */
/*   Updated: 2022/04/01 14:25:11 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

extern t_minishell		g_msh;

char	*search_in_directories(char	**directories, char *command, int *status)
{
	int		index;
	char	*executable;
	char	*absolute_path;

	index = 0;
	executable = ft_strjoin("/", command);
	while (directories[index] != NULL)
	{
		absolute_path = ft_strjoin(directories[index], executable);
		if (access(absolute_path, F_OK) == 0 && (command != NULL
				&& ft_strcmp(command, ".") != 0
				&& ft_strcmp(command, "..") != 0))
		{
			ft_free((void **)&executable);
			ft_free((void **)&command);
			return (absolute_path);
		}
		ft_free((void **)&absolute_path);
		index++;
	}
	*status = 1;
	ft_free((void **)&executable);
	return (command);
}

char	*find_executable(char *command)
{
	int		status;
	int		index;
	char	*path;
	char	**directories;

	status = 0;
	path = get_var("PATH");
	directories = ft_split(path, ':');
	if (!ft_strchr(command, '/') && path != NULL)
		command = search_in_directories(directories, command, &status);
	ft_free_array(&directories);
	ft_free((void **)&path);
	if (status == 1)
	{
		put_stderr(SHELL, command, NULL, "command not found");
		ft_free((void **)&command);
		ft_atexit(127);
	}
	return (command);
}
