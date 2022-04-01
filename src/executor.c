#include "../inc/minishell.h"

extern t_minishell	g_msh;

void	filestream_operations(int *initial_stdin, int *initial_stdout,
			int mode)
{
	if (mode == 1)
	{
		*initial_stdin = dup(STDIN_FILENO);
		*initial_stdout = dup(STDOUT_FILENO);
	}
	else if (mode == 2)
	{
		dup2(*initial_stdin, STDIN_FILENO);
		dup2(*initial_stdout, STDOUT_FILENO);
	}
	else if (mode == 3)
	{
		close(*initial_stdin);
		close(*initial_stdout);
	}
}

int	own_fork(pid_t *process_id)
{
	*process_id = fork();
	if (*process_id == -1)
	{
		perror("fork");
		return (-1);
	}
	return (0);
}

void	executioner(t_table *table)
{
	signal(SIGINT, SIG_IGN);
	if (table == NULL)
		return ;
	if (table->next == NULL)
		simple_command(table);
	else
		execute_pipeline(table);
}
