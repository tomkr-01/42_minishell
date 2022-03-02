/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/02 18:14:36 by tkruger           #+#    #+#             */
/*   Updated: 2022/03/02 18:16:47 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// This f() returns returns a malloced char ** with the added element and frees
// the old one
char	**add_array_element(char **old_arr, char *new_el)
{
	char	**new_arr;
	size_t	i;

	i = 0;
	new_arr = ft_calloc(ft_arrlen(old_arr) + 2, sizeof(*new_arr));
	if (new_arr == NULL)
		exit(printf("add_array_element(): malloc fail"));
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
	size_t	n;
	size_t	o;

	n = 0;
	o = 0;
	new_arr = ft_calloc(ft_arrlen(old_arr), sizeof(*new_arr));
	if (new_arr == NULL)
		exit(printf("rm_array_element(): malloc fail\n"));
	while (old_arr[o] != NULL)
	{
		if (old_arr[o] == old_el)
			o++;
		new_arr[n++] = old_arr[o++];
	}
	new_arr[n] = NULL;
	if (old_el != NULL)
		free(old_el);
	old_el = NULL;
	if (old_arr != NULL)
		free(old_arr);
	old_arr = NULL;
	return (new_arr);
}
