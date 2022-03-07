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

/* int array storing the beginning and ending of quotes? */

bool	can_be_expanded(char *argument)
{
	int			index;
	int			s_quotes;
	bool		expansion;

	index = 0;
	s_quotes = 1;
	expansion = false;
	while (argument[index] != '\0')
	{
		if (argument[index] == "'")
			s_quotes *= -1;
		else if (argument[index] == '$' && ft_isalnum(argument[index + 1]) && s_quotes > 0)
			expansion = true;
			
	}
}

char	*expansion(char *token, char **envp)
{
	int			index;
	int			start;
	int			var_len;
	char		*variable;
	char		to_remove;
	bool		expand;

	index = 0;
	expand = true;
	while (token[index] != '\0')
	{
		var_len = 0;
		if (token[index] == '"' || token[index] == "'")
			to_remove = token[index];
		while (token[index] != '\0' && token[index] != to_remove)
		{
			if (token[index++] == '$' && to_remove != "'")
			{
				start = index;
				while (token[index] != '\0' && ft_isalnum(token[index]))
					var_len++;
				variable = ft_substr(token, start, var_len);
				variable = ft_getenv(variable, envp);
				// append the string/character to the string before

			}
			index;
		}
	}
}
