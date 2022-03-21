#include "../inc/minishell.h"

extern t_minishell	g_msh;

int	env_builtin(char **arguments)
{
	size_t	i;

	i = 0;
	while (g_msh.env[i] != NULL)
	{
		ft_putendl_fd(g_msh.env[i], STDOUT_FILENO);
		i++;
	}
	if (arguments[0] == NULL)
		return (EXIT_SUCCESS);
	put_stderr(NULL, "env", arguments[0], strerror(ENOENT));
	return (127);
}
