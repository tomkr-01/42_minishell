/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 12:43:44 by tkruger           #+#    #+#             */
/*   Updated: 2022/03/01 21:12:42 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <errno.h>
#include <stdio.h>

extern char	**g_env;

int	echo_builtin(char const **arguments);
int	cd_builtin(char const **arguments);
int	pwd_builtin(char const **arguments);
int	export_builtin(char const **arguments);
int	unset_builtin(char const **arguments);
int	env_builtin(char const **arguments);
int	exit_builtin(char const **arguments);

int	check_builtins(char const **arguments)
{
	size_t	i;

	char *builtin_str[] = {
		"echo", "cd", "pwd", "export", "unset", "env", "exit"
	};
	int (*builtin_func[])(char const **) = {
		&echo_builtin, &cd_builtin, &pwd_builtin, &export_builtin,
		&unset_builtin, &env_builtin, &exit_builtin,
	};
	i = 0;
	while (builtin_str[i] != NULL)
	{
		if (ft_strncmp(arguments[0], builtin_str[i],
			ft_strlen(builtin_str[i]) == 0))
			return (*builtin_func[i](&arguments[1]));
	}
}

int	echo_builtin(char const **arguments)
{
	size_t	i;
	bool	is_option;

	i = 0;
	is_option = true;
	while (arguments != NULL && arguments[i] != NULL)
	{
		if (ft_strncmp(arguments[i], "-n", 3) != 0 || is_option == false)
		{
			is_option = false;
			ft_putstr_fd((char *)arguments[i], STDOUT_FILENO);
		}
		i++;
	}
	if (ft_strncmp(arguments[0], "-n", 3) != 0)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (EXIT_SUCCESS);
}

int	cd_builtin(char const **arguments)
{
	if (chdir(arguments[0]))
	{
		ft_putstr_fd((char *)arguments[0], STDOUT_FILENO);
		ft_putendl_fd(": No such file or directory", STDOUT_FILENO);
		return (ENOENT);
	}
	return (EXIT_SUCCESS);
}

int	pwd_builtin(char const **arguments)
{
	char	*pwd;
	size_t	i;

	i = 1;
	if (ft_arrlen((char **)arguments) > 0)
	{
		ft_putendl_fd("pwd: too many arguments", STDOUT_FILENO);
		return (E2BIG);
	}
	pwd = ft_calloc(i, sizeof(*pwd));
	if (pwd == NULL)
		return (ENOMEM);
	while (getcwd(pwd, i) == NULL)
	{
		free(pwd);
		i *= 10;
		pwd = ft_calloc(i, sizeof(*pwd));
		if (pwd == NULL)
			return (ENOMEM);
	}
	ft_putendl_fd(pwd, STDOUT_FILENO);
	free(pwd);
	pwd = NULL;
	return (EXIT_SUCCESS);
}

int	export_builtin(char const **arguments)
{
	return (0);
}

int	unset_builtin(char const **arguments)
{
	return (0);
}

int	env_builtin(char const **arguments)
{
	size_t	i;

	i = 0;
	while (g_env[i] != NULL)
	{
		if (arguments == NULL)
			ft_putendl_fd(g_env[i], STDOUT_FILENO);
		else if (ft_strncmp(g_env[i], arguments[0], ft_strlen(arguments[0])) == 0)
		{
			ft_putendl_fd(ft_strchr(g_env[i], '=') + 1, STDOUT_FILENO);
			return (EXIT_SUCCESS);
		}
		i++;
	}
	if (arguments == NULL)
		return (EXIT_SUCCESS);
	ft_putstr_fd("env: ", STDOUT_FILENO);
	ft_putendl_fd((char *)arguments[0], STDOUT_FILENO);
	return (127); // not sure about this but bash returns this code when there are arguments that do not fit
}

int	exit_builtin(char const **arguments)
{
	return (0);
}
