/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_scmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/01 14:25:06 by tkruger           #+#    #+#             */
/*   Updated: 2022/04/01 14:25:07 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	simple_command(t_table *table)
{
	int		status;
	int		initial_stdin;
	int		initial_stdout;
	pid_t	process_id;

	status = 0;
	filestream_operations(&initial_stdin, &initial_stdout, 1);
	execute_redirections(&table->redirections, NULL, &status);
	if (check_builtins(table->arguments))
	{
		builtins(table->arguments);
		filestream_operations(&initial_stdin, &initial_stdout, 2);
		return ;
	}
	else
	{
		if (own_fork(&process_id) == -1)
			return ;
		if (process_id == 0)
			execute_child(&table, &status);
	}
	wait_for_last(process_id, initial_stdin, initial_stdout);
	filestream_operations(&initial_stdin, &initial_stdout, 3);
}
