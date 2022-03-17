#include "../inc/minishell.h"

char	*read_input(void)
{
	char	*line;

	if (isatty(STDIN_FILENO) == 1)
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

/* signalhandler for child */

int	input_processor(char *line, t_table **table)
{
	t_list		*tokens;

	change_attributes(true);
	tokens = lexer(line);
	// if (tokens == NULL)
	// {
	// 	printf("return at lexer\n");
	// 	return (-1);
	// }
	free(line);
	line = NULL;
	syntax_check(tokens);
	*table = parser(tokens);
	ft_lstclear(&tokens, &del_content);
	if (*table == NULL)
	{
		printf("return at token\n");
		return (-1);
	}
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
		signal(SIGINT, control_c);
		change_attributes(false);
		line = read_input();
		if (line[0] =='\0')
			printf("line is: %s|\n", line);
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
		else if (line[0] != '\0')
		{
			status = input_processor(line, &table);
			executioner(table);
		}
	}
	return (0);
}
