/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/01 14:24:55 by tkruger           #+#    #+#             */
/*   Updated: 2022/04/01 14:24:56 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	set_attributes(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	change_attributes(true);
}

static void	execute_child_errors(t_table **table)
{
	int				exit_status;
	struct stat		buf;

	exit_status = 0;
	if (access((*table)->arguments[0], F_OK) == 0
		&& access((*table)->arguments[0], X_OK) != 0)
	{
		put_stderr(SHELL, (*table)->arguments[0], NULL, "permission denied");
		exit_status = 126;
	}
	else if (stat((*table)->arguments[0], &buf) == 0)
	{
		if (S_ISDIR(buf.st_mode))
		{
			put_stderr(SHELL, (*table)->arguments[0], NULL, "is a directory");
			exit_status = 126;
		}
	}
	else
	{
		put_stderr(SHELL, (*table)->arguments[0], NULL,
			"no such file or directory");
		exit_status = 127;
	}
	ft_atexit(exit_status);
}

void	execute_child(t_table **table, int *status)
{
	char	*command;

	if ((*table)->arguments == NULL)
		ft_atexit(*status);
	set_attributes();
	if (check_builtins((*table)->arguments))
	{
		builtins((*table)->arguments);
		ft_atexit(g_msh.exit_code);
	}
	command = find_executable(ft_strdup((*table)->arguments[0]));
	execve(command, (*table)->arguments, g_msh.env);
	ft_free((void **)&command);
	execute_child_errors(table);
}

static void	prepare_pipe(int **pipe_ends)
{
	close((*pipe_ends)[READ]);
	dup2((*pipe_ends)[WRITE], STDOUT_FILENO);
	close((*pipe_ends)[WRITE]);
}

void	child_process(t_table **table, int **pipe_ends, int *pipe_flag)
{
	int	status;

	status = 0;
	if (*pipe_flag == 1)
		prepare_pipe(pipe_ends);
	execute_redirections(&(*table)->redirections, pipe_ends, &status);
	if (pipe_ends != NULL)
	{
		free(*pipe_ends);
		*pipe_ends = NULL;
	}
	execute_child(table, &status);
}
