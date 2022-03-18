#include "../inc/minishell.h"
#include <signal.h>
#include <termios.h>


void	control_c(int sig)
{
	if (sig == SIGINT)
	{
		write(STDERR_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	execution_signals(int sig)
{
	if (sig == 0)
		rl_replace_line("", 0);
	else if (sig == SIGINT)
	{
		write(STDERR_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
	}
	else if (sig == SIGQUIT)
	{
		write(STDERR_FILENO, "Quit: 3\n", 8);
		rl_replace_line("", 0);
		rl_on_new_line();
	}
}

int	change_attributes(bool print_controls)
{
	int				err;
	struct termios	termios;

	err = tcgetattr(STDOUT_FILENO, &termios);
	if (err == -1)
		return (-1);
	if (print_controls)
		termios.c_lflag |= ECHOCTL;
	else
		termios.c_lflag &= ~(ECHOCTL);
	err = tcsetattr(STDOUT_FILENO, TCSANOW, &termios);
	if (err == -1)
		return (-1);
	return (0);
}
	   