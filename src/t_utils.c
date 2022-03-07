/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_utils.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 11:25:54 by tkruger           #+#    #+#             */
/*   Updated: 2022/03/07 19:57:50 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

#include <stdio.h>
#include <string.h> // for strerror
#include <errno.h>

int	export_builtin(char **arguments);

void	set_exit_code(int code)
{
	char	**set_code;

	set_code = ft_calloc(2, sizeof(*set_code));
	if (set_code == NULL)
		exit(put_stderr(SHELL, "set_exit_code", NULL, "mem allocation failed"));
	set_code[0] = ft_strjoin_free(ft_strdup("?="), ft_itoa(code));
	set_code[1] = NULL;
	export_builtin(set_code);
	free_array(&set_code);
}

// This f() returns returns a malloced char ** with the added element and frees
// the old one
char	**add_array_element(char **old_arr, char *new_el)
{
	char	**new_arr;
	size_t	i;

	i = 0;
	new_arr = ft_calloc(ft_arrlen(old_arr) + 2, sizeof(*new_arr));
	if (new_arr == NULL)
		exit(put_stderr(SHELL, "rm_array_element()", NULL, strerror(ENOMEM)));
	while (old_arr[i] != NULL)
	{
		new_arr[i] = old_arr[i];
		i++;
	}
	free(old_arr);
	old_arr = NULL;
	new_arr[i] = ft_strdup(new_el);
	new_arr[i + 1] = NULL;
	return (new_arr);
}

// This f() returns a malloced char ** with the removed element and frees the
// old one
char	**rm_array_element(char **old_arr, char	*old_el)
{
	char	**new_arr;
	size_t	new;
	size_t	old;

	new = 0;
	old = 0;
	printf("old_el: %s\n\n", old_el);
	new_arr = ft_calloc(ft_arrlen(old_arr), sizeof(*new_arr));
	if (new_arr == NULL)
		exit(put_stderr("rm_array_element(): malloc fail\n",
				NULL, NULL, strerror(ENOMEM)));
	while (old_arr[old] != NULL)
	{
		if (old_arr[old] == old_el)
			old++;
		new_arr[new++] = old_arr[old++];
	}
	new_arr[new] = NULL;
	if (old_el != NULL)
		free(old_el);
	old_el = NULL;
	if (old_arr != NULL)
		free(old_arr);
	old_arr = NULL;
	return (new_arr);
}

int	put_stderr(char	*shell_name, char *cmd, char *arg, char *message)
{
	if (shell_name != NULL)
	{
		ft_putstr_fd(shell_name, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (cmd != NULL)
	{
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (arg != NULL)
	{
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (message != NULL)
	{
		ft_putstr_fd(message, STDERR_FILENO);
	}
	ft_putchar_fd('\n', STDERR_FILENO);
	return (-1);
}
