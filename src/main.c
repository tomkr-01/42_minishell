#include "../inc/minishell.h"

char	*read_input(void)
{
	int		tty_flag;
	char	*line;

	tty_flag = isatty(STDIN_FILENO);
	if (tty_flag == 1)
	{
		line = readline("$> ");
		/* don't have to check for newline as readline removes the newline */
		if (line != NULL && line[0] != '\0')
			add_history(line);
	}
	else
		line = get_next_line(STDIN_FILENO);
	return (line);
}

int	input_processor(char *line, t_table **table)
{
	t_list		*tokens;
	t_table		*table;

	tokens = lexer(line);
	// if (tokens == NULL)
	// 	return (-1);
	free(line);
	*table = parser(tokens);
	if (*table == NULL)
		return (-1);
	return (0);
}

int	main(int argc, char *argv[], char **envp)
{
	int			status;
	char		*line;
	t_table		*table;

	// function to get the environment into own variable and check if it fails
	environment_init(envp);
	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
		/* sighandler for ctrl C -> display the prompt on a new line */
		signal(SIGINT, control_c);
		change_attributes(false);
		line = read_input();
		if (line == NULL)
		{
			/* this handles the case for ctrl d sending an EOF to the tty;
			-> line = NULL and therefore we have to quit and print exit on stdout/stderr?
			*/
			/* if you run: bash > out it displays exit on the terminal
				if you run bash 2> out (redirect stderr) the created file is empty
				maybe check for tty again?
			*/
			if (isatty(STDERR_FILENO) == 0)
				write(STDERR_FILENO, "exit\n", 5);
			return (-1);
		}
		status = input_processor(line, &table);
		if (status == -1)
		{
			// free stuff 
			return (-1);
		}
	}
	return (0);
}
