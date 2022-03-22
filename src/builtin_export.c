#include "../inc/minishell.h"

extern t_minishell	g_msh;
bool	invalid_identifier(char *variable);

int	export_builtin(char **arguments)
{
	size_t	a_i;
	size_t	i;

	a_i = 0;
	while (arguments[a_i] != NULL)
	{
		if (invalid_identifier(arguments[a_i]) == true)
		{
			a_i++;
			continue ;
		}
		i = 0;
		while (g_msh.env[i] != NULL && ft_strncmp(g_msh.env[i], arguments[a_i],
				ft_strchr_int(arguments[a_i], '=') + 1) != 0)
			i++;
		if (g_msh.env[i] != NULL)
		{
			g_msh.env[i] = ft_strdup(arguments[a_i]);
		}
		else
			g_msh.env = add_array_element(g_msh.env, arguments[a_i]);
		a_i++;
	}
	return (EXIT_SUCCESS);
}

bool	invalid_identifier(char *variable)
{
	size_t	i;

	i = 0;
	if (ft_strchr(variable, '=') == NULL)
	{
		put_stderr(SHELL, "export_builtin", variable, "not a valid identifier");
		return (true);
	}
	while (variable[i] != '=' || i == 0)
	{
		if (valid_exp_char(variable[i], false) == false)
		{
			put_stderr(SHELL, "export_builtin", variable, "not a valid identifier");
			return (true);
		}
		i++;
	}
	return (false);
}
