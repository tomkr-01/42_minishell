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

bool	input_processor(char *line, t_table **table)
{
	t_list		*tokens;

	change_attributes(true);
	tokens = lexer(line);
	if (tokens == NULL)
		return (false);
	ft_free((void **)&line);
	if (syntax_check(tokens) == false)
		return (false);
	*table = parser(tokens);
	if (*table == NULL)
		return (false);
	ft_lstclear(&tokens, &del_content);
	return (true);
}

int	main(__unused int argc, __unused char *argv[], char **envp)
{
	char	*line;
	t_table	*table;

	environment_init(envp);
	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, control_c);
		change_attributes(false);
		line = read_input();
		if (line == NULL)
		{
			if (isatty(STDERR_FILENO) == 0)
				write(STDERR_FILENO, "exit\n", 5);
			return (-1);
		}
		else if (line[0] != '\0')
		{
			if (input_processor(line, &table))
				executioner(table);
			// free table and set it to NULL otherwise it can accidentally run the insides of table a second time
			table = NULL;
		}
	}
	return (0);
}
