#include "../inc/minishell.h"

extern t_minishell g_msh;

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
	ft_lstclear(&tokens, &del_content);
	executioner(command_table);
	table_clear(&command_table);
}

#define PROMPT "$> "

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
			rl_clear_history();
			return (g_msh.exit_code);
		}
		handle_input(line);
	}
	return (0);
}
