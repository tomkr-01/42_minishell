#include "../inc/minishell.h"

int	pwd_builtin(char **arguments)
{
	char	*pwd;

	(void)arguments;
	pwd = getcwd(NULL, 0);
	ft_putendl_fd(pwd, STDOUT_FILENO);
	ft_free((void **)&pwd);
	return (EXIT_SUCCESS);
}
