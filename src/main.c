#include "../inc/minishell.h"

void	handle_input(char *line)
{
	t_list		*tokens;
	t_table		*command_table;

	tokens = NULL;
	command_table = NULL;
	// lexer frees the line
	tokens = lexer(line);
	if (tokens == NULL)
		return ;
	// if failing free the tokens
	// syntax check must be formed into bool with two options or int func
	if (!syntax_check(tokens))
		return ;
	command_table = parser(tokens);
	if (command_table == NULL)
		return ;
	executioner(command_table);
	ft_lstclear(&tokens, &del_content);
	// clear the command table
}

char	*read_input(void)
{
	char	*line;

	if (isatty(STDIN_FILENO) == 1)
	{
		line = readline("$> ");
		if (line != NULL && line[0] != '\0')
			add_history(line);
	}
	else
		line = get_next_line(STDIN_FILENO);
	return (line);
}

int	main(int argc, char *argv[], char **envp)
{
	char	*line;

	environment_init(envp);
	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, control_c);
		change_attributes(false);
		line = read_input();
		if (line == NULL)
		{
			if (isatty(STDERR_FILENO))
				write(STDERR_FILENO, "exit\n", 5);
			change_attributes(true);
			return (-1);
		}
		handle_input(line);
	}
	return (0);
}
