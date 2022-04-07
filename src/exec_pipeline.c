/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/01 14:24:59 by tkruger           #+#    #+#             */
/*   Updated: 2022/04/01 14:25:00 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	initialize_pipe(int **pipe_ends)
{
	*pipe_ends = (int *)malloc(2 * sizeof(int));
	if (*pipe_ends == 0)
		return (-1);
	return (0);
}

static int	pipe_found(t_table **table, int **pipe_ends)
{
	int	status;

	status = 0;
	if ((*table)->next != NULL)
	{
		status = pipe(*pipe_ends);
		if (status == -1)
			return (-1);
		return (1);
	}
	return (status);
}

static void	parent_process(int **pipe_ends, int *pipe_flag)
{
	if (*pipe_flag == 1)
	{
		close((*pipe_ends)[WRITE]);
		dup2((*pipe_ends)[READ], STDIN_FILENO);
		close((*pipe_ends)[READ]);
	}
}

void	execute_pipeline(t_table *table)
{
	int		initial_stdin;
	int		initial_stdout;
	int		pipe_flag;
	int		*pipe_ends;
	pid_t	process_id;

	if (initialize_pipe(&pipe_ends) == -1)
		return ;
	filestream_operations(&initial_stdin, &initial_stdout, 1);
	while (table != NULL)
	{
		pipe_flag = pipe_found(&table, &pipe_ends);
		if (pipe_flag == -1 || own_fork(&process_id) == -1)
			return ;
		if (process_id == 0)
			child_process(&table, &pipe_ends, &pipe_flag);
		else if (process_id > 0)
			parent_process(&pipe_ends, &pipe_flag);
		table = table->next;
	}
	ft_free((void **)&pipe_ends);
	wait_for_all(process_id, initial_stdin, initial_stdout);
	filestream_operations(&initial_stdin, &initial_stdout, 3);
}
