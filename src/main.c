#include "../inc/minishell.h"

extern t_minishell g_msh;

static char	*str_append_chr(char *str, char append);
static char	*minishell_gnl_free_line(char *line);

char	*minishell_get_next_line(int fd)
{
	char	*line;
	char	buffer;
	int		check;

	line = ft_strdup("");
	if (line == NULL)
		return (NULL);
	check = read(fd, &buffer, 1);
	if (check == -1 || check == 0)
		return (minishell_gnl_free_line(line));
	while (check > 0)
	{
		line = str_append_chr(line, buffer);
		if (line == NULL)
			return (NULL);
		if (buffer == '\n')
			return (line);
		check = read(fd, &buffer, 1);
	}
	if (check == -1)
		return (minishell_gnl_free_line(line));
	return (line);
}

static char	*str_append_chr(char *str, char append)
{
	char	*new_str;
	int		i;

	if (str == NULL)
		return (NULL);
	new_str = malloc(ft_strlen(str) + 2);
	if (new_str != NULL)
	{
		i = 0;
		while (str[i])
		{
			new_str[i] = str[i];
			i++;
		}
		new_str[i] = append;
		new_str[i + 1] = '\0';
	}
	free(str);
	return (new_str);
}

static char	*minishell_gnl_free_line(char *line)
{
	free(line);
	return (NULL);
}

t_table	*get_head(void)
{
	static t_table		head;

	return (&head);
}

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
	get_head()->next = command_table;
	ft_lstclear(&tokens, &del_content);
	executioner(command_table, command_table);
	table_clear(&command_table);
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
