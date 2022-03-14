/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 14:06:12 by tkruger           #+#    #+#             */
/*   Updated: 2022/03/14 19:31:34 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*get_pwd(void);
int		export_builtin(char **arguments);

int	cd_builtin(char **arguments)
{
	char	**export_pwds;

	if (arguments[0] == NULL)
		chdir(get_var("HOME"));
	else if (chdir(arguments[0]))
	{
		put_stderr(SHELL, "cd", arguments[0], strerror(ENOENT));
		return (EXIT_FAILURE);
	}
	export_pwds = ft_calloc(3, sizeof(*export_pwds));
	if (export_pwds == NULL)
		exit(put_stderr(SHELL, "cd_builtin()", NULL, strerror(ENOMEM)));
	export_pwds[0] = ft_strjoin_free(ft_strdup("OLDPWD="), get_var("PWD"));
	export_pwds[1] = ft_strjoin_free(ft_strdup("PWD="), get_pwd());
	export_pwds[2] = NULL;
	export_builtin(export_pwds);
	ft_free_array(&export_pwds);
	return (EXIT_SUCCESS);
}
