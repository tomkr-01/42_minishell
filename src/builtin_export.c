#include "../inc/minishell.h"

extern t_minishell	g_msh;
int		print_enrivonment(void);
void	has_assignment(char *argument, int *id_len, int *status);
int		is_valid_identifier(char *argument, int *id_len);
void	search_in_environment(char *argument, int *id_len, int *status);

int	export_builtin(char **arguments)
{
	int		i;
	int		id_len;
	int		status;
	int		exit_status;

	i = 0;
	exit_status = 0;
	if (arguments == NULL || arguments[0] == NULL)
		return (print_enrivonment());
	while (arguments[i] != NULL)
	{
		status = 0;
		if ((ft_isalpha(arguments[i][0]) || arguments[i][0] == '_'))
			has_assignment(arguments[i], &id_len, &status);
		else
		{
			status = 1;
			exit_status = EXIT_FAILURE;
		}
		if (status == 1)
			put_stderr(SHELL, "export_builtin", arguments[i],
				"not a valid identifier");
		i++;
	}
	return (exit_status);
}

int	print_enrivonment(void)
{
	int		index;

	index = 0;
	if (g_msh.env == NULL)
		return (0);
	while (g_msh.env[index] != NULL)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putendl_fd(g_msh.env[index], STDOUT_FILENO);
		index++;
	}
	return (0);
}

void	has_assignment(char *argument, int *id_len, int *status)
{
	if (ft_strchr(argument, '='))
	{
		*id_len = 1;
		*status = is_valid_identifier(argument, id_len);
		search_in_environment(argument, id_len, status);
		if (*status == 0)
			g_msh.env = add_array_element(g_msh.env, argument);
	}
}

int	is_valid_identifier(char *argument, int *id_len)
{
	while (argument[*id_len] != '=')
	{
		if (!ft_isalnum(argument[*id_len]) && argument[*id_len] != '_')
			return (1);
		*id_len += 1;
	}
	return (0);
}

void	search_in_environment(char *argument, int *id_len, int *status)
{
	int		i;

	if (g_msh.env == NULL)
		return ;
	i = 0;
	while (g_msh.env[i] != NULL && *status == 0)
	{
		if (ft_strncmp(g_msh.env[i], argument, *id_len + 1) == 0)
		{
			free(g_msh.env[i]);
			g_msh.env[i] = ft_strdup(argument);
			*status = 2;
			break ;
		}
		i++;
	}
}
