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

/* own get_env function */

char	*ft_getenv(const char *variable, char **envp)
{
	int		index;
	int		variable_len;
	char	*environment_variable;

	index = 0;
	environment_variable = ft_strjoin(variable, "=");
	variable_len = ft_strlen(environment_variable);
	while (envp[index])
	{
		if (ft_strncmp(envp[index], environment_variable, variable_len) == 0)
		{
			free(environment_variable);
			environment_variable = NULL;
			return (envp[index] + variable_len);
		}
		index++;
	}
	free(environment_variable);
	environment_variable = NULL;
	return (NULL);
}

/* variable expansion function */

/* helper function like
	str_apppend_char
	str_append_str
*/

size_t	protected_strlen(char *s)
{
	int		index;

	index = 0;
	if (s == NULL)
		return (0);
	while (s[index] != '\0')
		index++;
	return (index);
}
