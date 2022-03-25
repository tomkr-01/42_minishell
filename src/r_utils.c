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
		ft_free((void **)&string);
	return (new);
}

// void	ft_free(void **ptr)
// {
// 	if (*ptr != NULL)
// 		free(*ptr);
// 	*ptr = NULL;
// }

char	*find_executable(char *command)
{
	int		index;
	char	*path;
	char	*executable;
	char	*absolute_path;
	char	**directories;
	struct stat	*statbuf;

	// didn't work previously because statbuf wasn't allocated, either this way or on the stack
	statbuf = (struct stat *)malloc(sizeof(struct stat));
	if (statbuf == NULL)
		return (NULL);
	if (stat(command, statbuf) == 0)
	{
		if (statbuf->st_mode & S_IFDIR)
		{
			printf("this is a directory: %s\n", command);
			return (NULL);
		}
	}
	// if (access(command, F_OK) == 0)
	// {
	// 	if (ft_strncmp(command, "./", 2) != 0)
	// 		return (NULL);
	// 	else
	// 		return (command);
	// }
	index = 0;
	path = get_var("PATH");
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
			ft_free((void **)&executable);
			ft_free((void **)&directories);
			// ft_free((void **)&command);
			return (absolute_path);
		}
		ft_free((void **)&absolute_path);
		index++;
	}
	ft_free((void **)&executable);
	ft_free((void **)&directories);
	return (command);
}
