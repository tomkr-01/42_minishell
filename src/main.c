#include "../inc/minishell.h"

extern t_minishell	g_msh;

static void	handle_input(char *line)
{
	t_list		*tokens;

	tokens = NULL;
	tokens = lexer(line);
	if (tokens == NULL)
		return ;
	if (!syntax_check(tokens))
	{
		ft_lstclear(&tokens, &del_content);
		return ;
	}
	get_head()->next = parser(tokens);
	ft_lstclear(&tokens, &del_content);
	executioner(get_head()->next);
	table_clear(&(get_head()->next));
}

static char	*read_input(void)
{
	char	*line;

	if (isatty(STDIN_FILENO) == 1)
	{
		line = readline(PROMPT);
		if (line != NULL && line[0] != '\0')
			add_history(line);
	}
	else
		line = minishell_get_next_line(STDIN_FILENO);
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
			ft_free_array(&g_msh.env);
			rl_clear_history();
			return (g_msh.exit_code);
		}
		handle_input(line);
	}
	return (0);
}
