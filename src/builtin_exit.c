#include "../inc/minishell.h"

int	exit_builtin(char **arguments)
{
	if (isatty(STDOUT_FILENO))
		ft_putendl_fd("exit", STDERR_FILENO);
	if (ft_arrlen(arguments) > 0 && !ft_isint(arguments[0]))
	{
		put_stderr(SHELL, "exit", arguments[0], "numeric argument required");
		exit((unsigned char)255);
	}
	else if (ft_arrlen(arguments) > 1)
	{
		put_stderr(SHELL, "exit", NULL, "too many arguments");
		return (EPERM);
	}
	else if (ft_arrlen(arguments) == 1)
		exit((unsigned char)ft_atoi(arguments[0]));
	else
		exit(EXIT_SUCCESS);
	return (EXIT_SUCCESS);
}
