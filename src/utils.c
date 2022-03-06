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
	while (first[i] != NULL)
		new_array[i] = ft_strdup(first[i]);
	while (second[i] != NULL)
		new_array[i + j] = ft_strdup(second[j]);
	new_array[i + j] = NULL;
	/* to possibilities
		1. free while copying
		2. call ft_free_split onto the old arrays
	*/
	return (new_array);
}
