/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd_echo_pwd.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/01 12:43:44 by tkruger           #+#    #+#             */
/*   Updated: 2022/03/07 13:10:43 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <errno.h>
#include <string.h> // for strerror()

extern char	**g_env;

int		cd_builtin(char **arguments);
int		echo_builtin(char **arguments);
int		pwd_builtin(char **arguments);
char	*pwd_helper(void);
int		export_builtin(char **arguments);
int		unset_builtin(char **arguments);
int		env_builtin(char **arguments);
int		exit_builtin(char **arguments);

int	builtins(char **arguments)
{
	int	exit_code;

	if (strcmp(arguments[0], "cd") == 0)
		exit_code = cd_builtin(&arguments[1]);
	else if (strcmp(arguments[0], "echo") == 0)
		exit_code = echo_builtin(&arguments[1]);
	else if (strcmp(arguments[0], "env") == 0)
		exit_code = env_builtin(&arguments[1]);
	else if (strcmp(arguments[0], "exit") == 0)
		exit_code = exit_builtin(&arguments[1]);
	else if (strcmp(arguments[0], "export") == 0)
		exit_code = export_builtin(&arguments[1]);
	else if (strcmp(arguments[0], "unset") == 0)
		exit_code = unset_builtin(&arguments[1]);
	else if (strcmp(arguments[0], "pwd") == 0)
		exit_code = pwd_builtin(&arguments[1]);
	else
		return (0);
	set_exit_code(exit_code);
	return (1);
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
		exit(put_stderr(SHELL, "cd_builtin()", NULL, strerror(ENOMEM)));
	export_pwds[0] = ft_strjoin_free(ft_strdup("OLDPWD="), get_var("PWD"));
	export_pwds[1] = ft_strjoin_free(ft_strdup("PWD="), pwd_helper());
	export_pwds[2] = NULL;
	export_builtin(export_pwds);
	free_array(&export_pwds);
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
		exit(put_stderr(SHELL, "pwd_helper()", NULL, strerror(ENOMEM)));
	while (getcwd(pwd, i) == NULL)
	{
		free(pwd);
		i *= 10;
		pwd = ft_calloc(i, sizeof(*pwd));
		if (pwd == NULL)
			exit(put_stderr(SHELL, "pwd_helper()", NULL, strerror(ENOMEM)));
	}
	return (pwd);
}
