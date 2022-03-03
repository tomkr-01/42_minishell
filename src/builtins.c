/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomkrueger <tomkrueger@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 12:43:44 by tkruger           #+#    #+#             */
/*   Updated: 2022/03/03 02:30:31 by tomkrueger       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <errno.h>
#include <stdio.h>

extern char	**g_env;

int		echo_builtin(char const **arguments);
int		cd_builtin(char const **arguments);
int		pwd_builtin(char const **arguments);
char	*pwd_helper(void);
int		export_builtin(char const **arguments);
int		unset_builtin(char const **arguments);
int		env_builtin(char const **arguments);
int		exit_builtin(char const **arguments);

int	check_builtins(char const **arguments)
{
	char	**builtin_str;
	size_t	i;

	builtin_str = ft_split("echo;cd;pwd;export;unset;env;exit", ';');
	int (*builtin_func[])(char const **) = {
		&echo_builtin, &cd_builtin, &pwd_builtin, &export_builtin,
		&unset_builtin, &env_builtin, &exit_builtin,
	};
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
			printf("%s ", arguments[i]);
		}
		i++;
	}
	if (ft_strncmp(arguments[0], "-n", 3) != 0)
		printf("\n");
	return (EXIT_SUCCESS);
}

int	cd_builtin(char const **arguments)
{
	char const	**export_arr;
	size_t		i;

	if (chdir(arguments[0]))
	{
		printf("%s: No such file or directory\n", arguments[0]);
		return (ENOENT);
	}
	export_arr = ft_calloc(3, sizeof(*export_arr));
	if (export_arr == NULL)
		exit(ENOMEM);
	export_arr[0] = ft_strjoin_free(ft_strdup("OLDPWD="), get_var("PWD"));
	export_arr[1] = ft_strjoin_free(ft_strdup("PWD="), pwd_helper());
	export_arr[2] = NULL;
	export_builtin(export_arr);
	free_array((char ***)&export_arr);
	return (EXIT_SUCCESS);
}

int	pwd_builtin(char const **arguments)
{
	char	*pwd;

	if (ft_arrlen((char **)arguments) > 0)
	{
		ft_putendl_fd("pwd: too many arguments", STDOUT_FILENO);
		return (E2BIG);
	}
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

int	export_builtin(char const **arguments)
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
			g_env = add_array_element(g_env, (char *)arguments[a_i]);
		}
		a_i++;
	}
	return (EXIT_SUCCESS);
}

int	unset_builtin(char const **arguments)
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
			g_env = rm_array_element(g_env, g_env[i]);
		}
		a_i++;
	}
	return (EXIT_SUCCESS);
}

int	env_builtin(char const **arguments)
{
	size_t	i;

	i = 0;
	while (g_env[i] != NULL)
	{
		if (ft_strncmp(g_env[i], "?", 2) != 0)
			printf("%s\n", g_env[i]);
		i++;
	}
	if (arguments == NULL)
		return (EXIT_SUCCESS);
	printf("env: %s: No such file or directory\n", arguments[0]);
	return (127); // not sure about this but bash returns this code when there are arguments that do not fit
}

int	exit_builtin(char const **arguments)
{
	exit(EXIT_SUCCESS);
	return (EXIT_SUCCESS);
}
