#include "../inc/minishell.h"

void	wait_for_last(int pid, int initial_stdin, int initial_stdout)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			ft_putstr_fd("\n", 2);
		else if (WTERMSIG(status) == SIGQUIT)
			ft_putstr_fd("Quit: 3\n", 2);
		g_msh.exit_code = 128 + WTERMSIG(status);
	}
	else if (WIFEXITED(status))
		g_msh.exit_code = WEXITSTATUS(status);
	filestream_operations(&initial_stdin, &initial_stdout, 2);
}

void	wait_for_all(int pid, int initial_stdin, int initial_stdout)
{
	wait_for_last(pid, initial_stdin, initial_stdout);
	while (wait(NULL) != -1)
		filestream_operations(&initial_stdin, &initial_stdout, 2);
}
