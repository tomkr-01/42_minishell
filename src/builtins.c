/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 12:43:44 by tkruger           #+#    #+#             */
/*   Updated: 2022/03/04 20:19:19 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <errno.h>
#include <string.h> // for strerror()

extern char	**g_env;

int		echo_builtin(char **arguments);
int		cd_builtin(char **arguments);
int		pwd_builtin(char **arguments);
char	*pwd_helper(void);
int		export_builtin(char **arguments);
int		unset_builtin(char **arguments);
int		env_builtin(char **arguments);
int		exit_builtin(char **arguments);

int	check_builtins(char **arguments)
{
	size_t	i;
	char	*builtin_str[] = {"echo", "cd", "pwd", "export", "unset", "env",
		"exit", NULL};
	int		(*builtin_func[])(char **) = {&echo_builtin, &cd_builtin,
		&pwd_builtin, &export_builtin, &unset_builtin, &env_builtin,
		&exit_builtin};

	i = 0;
	while (builtin_str[i] != NULL)
	{
		if (ft_strncmp(arguments[0], builtin_str[i],
				ft_strlen(builtin_str[i])) == 0)
			return ((*builtin_func[i])(&arguments[1]));
		i++;
	}
	return (-1000);
}

int	echo_builtin(char **arguments)
{
	size_t	i;
	size_t	j;
	int		new_line;

	i = 0;
	new_line = 1;
	while (arguments != NULL && arguments[i] != NULL)
	{
		j = 1;
		while (arguments[i][0] == '-' && arguments[i][j] == 'n')
			j++;
		if (arguments[i][j] == '\0' && new_line > 0)
			new_line = 2;
		else
		{
			if (new_line > 0)
				new_line *= -1;
			ft_putstr_fd(arguments[i], STDOUT_FILENO);
			ft_putchar_fd(' ', STDOUT_FILENO);
		}
		i++;
	}
	if (new_line == -1)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (EXIT_SUCCESS);
}

int	cd_builtin(char **arguments)
{
	char	**export_pwds;

	if (arguments[0] == NULL)
		chdir(get_var("HOME"));
	else if (chdir(arguments[0]))
	{
		put_stderr(SHELL, "cd", arguments[0], strerror(ENOENT));
		return (1); // somehow the same value as in bash, not 2 as in ENOENT
	}
	export_pwds = ft_calloc(3, sizeof(*export_pwds));
	if (export_pwds == NULL)
		exit(ENOMEM);
	export_pwds[0] = ft_strjoin_free(ft_strdup("OLDPWD="), get_var("PWD"));
	export_pwds[1] = ft_strjoin_free(ft_strdup("PWD="), pwd_helper());
	export_pwds[2] = NULL;
	export_builtin(export_pwds);
	free_array((char ***)&export_pwds);
	return (EXIT_SUCCESS);
}

int	pwd_builtin(char **arguments)
{
	char	*pwd;

	pwd = pwd_helper();
	ft_putendl_fd(pwd, STDOUT_FILENO);
	if (pwd != NULL)
		free(pwd);
	pwd = NULL;
	return (EXIT_SUCCESS);
}

char	*pwd_helper(void)
{
	char	*pwd;
	size_t	i;

	i = 1;
	pwd = ft_calloc(i, sizeof(*pwd));
	if (pwd == NULL)
		exit(ENOMEM);
	while (getcwd(pwd, i) == NULL)
	{
		free(pwd);
		i *= 10;
		pwd = ft_calloc(i, sizeof(*pwd));
		if (pwd == NULL)
			exit(ENOMEM);
	}
	return (pwd);
}

int	export_builtin(char **arguments)
{
	size_t	a_i;
	size_t	i;

	a_i = 0;
	while (arguments[a_i] != NULL)
	{
		i = 0;
		while (g_env[i] != NULL && ft_strncmp(g_env[i], arguments[a_i],
				ft_strchr_int(arguments[a_i], '=')) != 0)
			i++;
		if (g_env[i] != NULL)
		{
			free(g_env[i]);
			g_env[i] = ft_strdup(arguments[a_i]);
		}
		else
		{
			g_env = add_array_element(g_env, arguments[a_i]);
		}
		a_i++;
	}
	return (EXIT_SUCCESS);
}

int	unset_builtin(char **arguments)
{
	size_t	a_i;
	size_t	i;

	a_i = 0;
	while (arguments[a_i] != NULL)
	{
		i = 0;
		while (g_env[i] != NULL && ft_strncmp(g_env[i], arguments[a_i],
				ft_strchr_int(arguments[a_i], '=')) != 0)
		{
			i++;
		}
		if (g_env[i] != NULL)
		{
			g_env = rm_array_element(g_env, g_env[i]);
		}
		a_i++;
	}
	return (EXIT_SUCCESS);
}

int	env_builtin(char **arguments)
{
	size_t	i;

	i = 0;
	while (g_env[i] != NULL)
	{
		if (ft_strncmp(g_env[i], "?", 2) != 0)
			ft_putendl_fd(g_env[i], STDOUT_FILENO);
		i++;
	}
	if (arguments == NULL)
		return (EXIT_SUCCESS);
	put_stderr(NULL, "env", arguments[0], strerror(ENOENT));
	return (127);
}

int	exit_builtin(char **arguments)
{
	ft_putendl_fd("exit", STDERR_FILENO);
	if (ft_arrlen(arguments) > 0 && !ft_isint(arguments[0]))
	{
		put_stderr(SHELL, "exit", arguments[0], "numeric argument required");
		exit(255);
	}
	else if (ft_arrlen(arguments) > 1)
	{
		put_stderr(SHELL, "exit", NULL, "too many arguments");
		return (EPERM);
	}
	else if (ft_arrlen(arguments) == 1)
	{
		exit(ft_atoi(arguments[0]));
	}
	else
	{
		exit(EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}
