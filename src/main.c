#include "../inc/minishell.h"

void	handle_input(char *line)
{
	t_list		*tokens;
	t_table		*command_table;

	tokens = NULL;
	command_table = NULL;
	tokens = lexer(line);
	if (tokens == NULL)
		return ;
	if (!syntax_check(tokens))
	{
		ft_lstclear(&tokens, &del_content);
		return ;
	}
	command_table = parser(tokens);
	executioner(command_table);
	if (command_table != NULL)
		free_command_table(command_table);
	else if (tokens != NULL)
		ft_lstclear(&tokens, &del_content);
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
