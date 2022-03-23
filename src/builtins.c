#include "../inc/minishell.h"

extern t_minishell	g_msh;

int		cd_builtin(char **arguments);
int		echo_builtin(char **arguments);
int		pwd_builtin(char **arguments);
int		export_builtin(char **arguments);
int		unset_builtin(char **arguments);
int		env_builtin(char **arguments);
int		exit_builtin(char **arguments);

bool	check_builtins(char **arguments)
{
	if (arguments == NULL)
		return (false);
	if (ft_strcmp(arguments[0], "cd") == 0
		|| ft_strcmp(arguments[0], "echo") == 0
		|| ft_strcmp(arguments[0], "env") == 0
		|| ft_strcmp(arguments[0], "exit") == 0
		|| ft_strcmp(arguments[0], "export") == 0
		|| ft_strcmp(arguments[0], "unset") == 0
		|| ft_strcmp(arguments[0], "pwd") == 0)
		return (true);
	else
		return (false);
}

void	builtins(char **arguments)
{
	if (ft_strcmp(arguments[0], "cd") == 0)
		g_msh.exit_code = cd_builtin(&arguments[1]);
	else if (ft_strcmp(arguments[0], "echo") == 0)
		g_msh.exit_code = echo_builtin(&arguments[1]);
	else if (ft_strcmp(arguments[0], "env") == 0)
		g_msh.exit_code = env_builtin(&arguments[1]);
	else if (ft_strcmp(arguments[0], "exit") == 0)
		g_msh.exit_code = exit_builtin(&arguments[1]);
	else if (ft_strcmp(arguments[0], "export") == 0)
		g_msh.exit_code = export_builtin(&arguments[1]);
	else if (ft_strcmp(arguments[0], "unset") == 0)
		g_msh.exit_code = unset_builtin(&arguments[1]);
	else if (ft_strcmp(arguments[0], "pwd") == 0)
		g_msh.exit_code = pwd_builtin(&arguments[1]);
}
