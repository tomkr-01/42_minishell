/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 14:07:24 by tkruger           #+#    #+#             */
/*   Updated: 2022/03/14 14:17:47 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*get_pwd(void);

int	pwd_builtin(char **arguments)
{
	char	*pwd;

	pwd = get_pwd();
	ft_putendl_fd(pwd, STDOUT_FILENO);
	if (pwd != NULL)
		free(pwd);
	pwd = NULL;
	return (EXIT_SUCCESS);
}

char	*get_pwd(void)
{
	char	*pwd;
	size_t	i;

	i = 1;
	pwd = ft_calloc(i, sizeof(*pwd));
	if (pwd == NULL)
		exit(put_stderr(SHELL, "get_pwd()", NULL, strerror(ENOMEM)));
	while (getcwd(pwd, i) == NULL)
	{
		free(pwd);
		i *= 10;
		pwd = ft_calloc(i, sizeof(*pwd));
		if (pwd == NULL)
			exit(put_stderr(SHELL, "get_pwd()", NULL, strerror(ENOMEM)));
	}
	return (pwd);
}
