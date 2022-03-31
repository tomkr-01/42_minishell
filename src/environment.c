#include "../inc/minishell.h"

t_minishell		g_msh;

void	environment_init(char **envp)
{
	size_t	i;

	i = 0;
	if (envp == NULL)
		return ;
	g_msh.env = ft_calloc((ft_arrlen(envp) + 1), sizeof(*g_msh.env));
	if (g_msh.env == NULL)
		exit(put_stderr(SHELL, "environment_init()", NULL, strerror(ENOMEM)));
	while (envp[i] != NULL)
	{
		g_msh.env[i] = ft_strdup(envp[i]);
		i++;
	}
	g_msh.env[i] = NULL;
	g_msh.exit_code = 0;
}

char	*get_var(const char *variable)
{
	int		index;
	int		var_len;
	char	*env_var;

	index = 0;
	env_var = ft_strjoin(variable, "=");
	var_len = ft_strlen(env_var);
	while (g_msh.env[index])
	{
		if (ft_strncmp(g_msh.env[index], env_var, var_len) == 0)
		{
			ft_free((void **)&env_var);
			return (ft_strdup(g_msh.env[index] + var_len));
		}
		index++;
	}
	ft_free((void **)&env_var);
	return (NULL);
}
