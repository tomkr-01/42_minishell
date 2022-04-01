/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_array.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/01 14:26:32 by tkruger           #+#    #+#             */
/*   Updated: 2022/04/01 14:26:33 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	**add_array_element(char **old_arr, char *new_el)
{
	char	**new_arr;
	size_t	i;

	i = 0;
	new_arr = ft_calloc(ft_arrlen(old_arr) + 2, sizeof(*new_arr));
	if (new_arr == NULL)
		exit(put_stderr(SHELL, "add_array_element()", NULL, "hallo"));
	while (old_arr != NULL && old_arr[i] != NULL)
	{
		new_arr[i] = old_arr[i];
		i++;
	}
	ft_free((void **)&old_arr);
	new_arr[i] = ft_strdup(new_el);
	new_arr[i + 1] = NULL;
	return (new_arr);
}

char	**rm_array_element(char **old_arr, char	*old_el)
{
	char	**new_arr;
	size_t	new;
	size_t	old;

	new = 0;
	old = 0;
	new_arr = ft_calloc(ft_arrlen(old_arr), sizeof(*new_arr));
	if (new_arr == NULL)
		exit(put_stderr(SHELL, "rm_array_element()", NULL, "hallo"));
	while (old_arr[old] != NULL)
	{
		if (old_arr[old] == old_el)
			old++;
		else
			new_arr[new++] = old_arr[old++];
	}
	new_arr[new] = NULL;
	ft_free((void **)&old_el);
	ft_free((void **)&old_arr);
	return (new_arr);
}

char	**array_append_array(char **first, char **second)
{
	int	i;

	i = 0;
	if (second == NULL)
		return (first);
	while (second != NULL && second[i] != NULL)
	{
		first = add_array_element(first, second[i]);
		i++;
	}
	ft_free_array(&second);
	return (first);
}
