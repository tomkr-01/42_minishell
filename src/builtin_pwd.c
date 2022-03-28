#include "../inc/minishell.h"

int	pwd_builtin(char **arguments)
{
	char	*pwd;

	if (arguments == NULL)
		;
	pwd = getcwd(NULL, 0);
	ft_putendl_fd(pwd, STDOUT_FILENO);
	ft_free((void **)&pwd);
	return (EXIT_SUCCESS);
}

// what does the if condition in line 7 do??