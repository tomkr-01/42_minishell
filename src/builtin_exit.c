#include "../inc/minishell.h"
#include <limits.h>

int	ft_is_numeric(char *arguments)
{
	int		index;
	int		status;

	index = 0;
	status = 0;
	if (arguments == NULL)
		return (1);
	if (arguments[index] == '\0')
		status = 1;
	else if (arguments[index] == '+' || arguments[index] == '-')
	{
		if (ft_isdigit(arguments[++index]) == 0)
			status = 1;
	}
	while (arguments[index] != '\0' && status == 0)
	{
		if (!ft_isdigit(arguments[index++]) || ft_strlen(arguments) > 19)
			status = 1;
	}
	if (status == 1)
		return (0);
	return (1);
}

int	exit_builtin(char **arguments)
{
	ft_putendl_fd("exit", STDERR_FILENO);
	if (!ft_is_numeric(arguments[0]))
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
