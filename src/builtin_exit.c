#include "../inc/minishell.h"

extern t_minishell	g_msh;

int	is_non_numeric(char *arguments)
{
	int		index;

	index = 0;
	if (arguments == NULL)
		return (0);
	if (arguments[0] == '\0')
		return (1);
	while (arguments[index] != '\0')
	{
		if (index == 0 && (arguments[index] == '+' || arguments[index] == '-'))
			;
		else if (index > 0
			&& (arguments[index] == '+' || arguments[index] == '-'))
			return (1);
		else if (!ft_isdigit(arguments[index]))
			return (1);
		index++;
	}
	return (0);
}

int	exit_builtin(char **arguments)
{
	if (isatty(STDERR_FILENO))
		ft_putendl_fd("exit", STDERR_FILENO);
	if (is_non_numeric(arguments[0]))
	{
		put_stderr(SHELL, "exit", arguments[0], "numeric argument required");
		ft_atexit((unsigned char)255);
	}
	else if (ft_arrlen(arguments) > 1)
	{
		put_stderr(SHELL, "exit", NULL, "too many arguments");
		return (1);
	}
	else if (ft_arrlen(arguments) == 1)
		ft_atexit(ft_atoi(arguments[0]));
	ft_atexit(g_msh.exit_code);
	return (0);
}
