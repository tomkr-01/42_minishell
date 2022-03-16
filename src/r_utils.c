/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   r_utils.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 11:16:55 by tkruger           #+#    #+#             */
/*   Updated: 2022/03/16 13:12:14 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	**array_append_array(char **first, char **second)
{
	int		i;
	int		j;
	char	**new_array;
	i = 0;
	j = 0;
	new_array = (char **)malloc((ft_arrlen(first) + ft_arrlen(second) + 1)
			* sizeof(char *));
	if (new_array == NULL)
		return (NULL);
	while (first != NULL && first[i] != NULL)
	{
		new_array[i] = ft_strdup(first[i]);
		i++;
	}
	while (second != NULL && second[j] != NULL)
	{
		new_array[i + j] = ft_strdup(second[j]);
		j++;
	}
	new_array[i + j] = NULL;
	/* to possibilities
		but check before if they are NULL
		1. free while copying
		2. call ft_free_split onto the old arrays
	*/
	return (new_array);
}
char	*str_append_char(char *string, char c)
{
	int			index;
	int			str_len;
	char		*new;

	if (string == NULL)
	{
		new = (char *)malloc(2 * sizeof(char));
		if (new == NULL)
		return (NULL);
		new[0] = c;
		new[1] = '\0';
		return (new);
	}
	index = 0;
	str_len = ft_strlen(string);
	new = (char *)malloc((str_len + 2) * sizeof(char));
	if (new == NULL)
		return (NULL);
	while (string[index] != '\0')
	{
		new[index] = string[index];
		index++;
	}
	new[index] = c;
	new[++index] = '\0';
	if (string != NULL)
		free(string);
	return (new);
}

void	ft_free(void **ptr)
{
	free(*ptr);
	*ptr = NULL;
}

char	*find_executable(char *command)
{
	int		index;
	char	*path;
	char	*executable;
	char	*absolute_path;
	char	**directories;

	if (access(command, F_OK) == 0)
		return (command);
	index = 0;
	path = "/Users/rjasari/.brew/bin:/Users/rjasari/.brew/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/munki:/Users/rjasari/.brew/bin";
	directories = ft_split(path, ':');
	if (directories == NULL)
		return (NULL);
	// free(path);
	executable = ft_strjoin("/", command);
	while (directories[index] != NULL)
	{
		absolute_path = ft_strjoin(directories[index], executable);
		if (access(absolute_path, F_OK) == 0)
		{
			free(executable);
			free(directories);
			free(command);
			return (absolute_path);
		}
		free(absolute_path);
		index++;
	}
	free(executable);
	free(directories);
	return (command);
}
