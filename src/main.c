#include "../inc/minishell.h"

int	main(int argc, char *argv[], char **envp)
{
	char	*line;

	// function to get the environment into own variable and check if it fails
	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
		/* sighandler for ctrl C -> display the prompt on a new line */
		signal(SIGINT, control_c);
		change_attributes(false);
	}
	return (0);
}
