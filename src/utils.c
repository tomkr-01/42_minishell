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
			return (ft_strdup(envp[index] + variable_len));
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
	-> ft_strjoin() can do the job
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

int	valid_expansion_character(int c)
{
	if (ft_isalnum(c) || c == '_')
		return (1);
	return (0);
}

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
		if (argument[index] == '\'')
			s_quotes *= -1;
		else if (argument[index] == '$'
			&& valid_expansion_character(argument[index + 1]) && s_quotes > 0)
			return (true);
		index++;
	}
	return (false);
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
	if (ptr == NULL || *ptr == NULL)
		return ;
	free(*ptr);
	*ptr = NULL;
}

ft_free_array(void ***to_free)
{
	size_t		i;

	if (to_free == NULL || *to_free == NULL)
		return ;
	i = 0;
	while ((*to_free)[i] != NULL)
	{
		ft_free((*to_free)[i]);
		(*to_free)[i] = NULL;
		i++;
	}
	free(*to_free);
	*to_free = NULL;
}

char	*find_expanding_index(char *token)
{
	while (*token)
	{
		if (*token == '\'')
		{
			token = ft_strchr(token + 1, '\'');
			if (token == NULL)
				return (NULL);
		}
		else if (*token == '"')
		{
			while (*++token != '\0' && *token != '"')
				if (*token == '$')
					return (token);
			if (token == NULL)
				return (NULL);
		}
		else if (*token == '$')
			return (token);
		token++;
	}
	return (NULL);
}

int	get_parameter(char *string, char **parameter)
{
	int		index;

	// we start at 1 because we wanna look at character next to $
	index = 1;
	*parameter = NULL;
	if (string == NULL)
		return (0);
	if (string[index] == '\0')
		return (0);
	if (string[index] == '?')
	{
		*parameter = ft_substr(string, 1, 1);
		return (1);
	}
	if (!ft_isalpha(string[index]) && string[index] != '_')
		return (0);
	while (string[index] != '\0')
	{
		if (!valid_expansion_character(string[index]))
			break ;
		*parameter = str_append_char(*parameter, string[index]);
		index++;
	}
	return (index - 1);
}

int	expansion(char	*token, char **expanded, char **envp)
{
	int		index;
	char	*temporary;
	char	*parameter;
	char	*partial;
	char	*expanded_param;
	char	*full_expansion;

	temporary = find_expanding_index(token);
	if (temporary == NULL)
		return (0);
	while (temporary)
	{
		parameter = NULL;
		partial = NULL;
		expanded_param = NULL;
		full_expansion = NULL;
		if (full_expansion != NULL)
			free(full_expansion);
		partial = ft_substr(token, 0, temporary - token);
		index += get_parameter(temporary, &parameter);
		if (parameter != NULL)
		{
			expanded_param = ft_getenv(parameter, envp);
			full_expansion = ft_strjoin(partial, expanded_param);
		}
		else
		{
			full_expansion = partial;
			free(partial);
		}
		partial = ft_substr(token, index, ft_strlen(temporary) - (index + 1));
		full_expansion = ft_strjoin(full_expansion, partial);
		temporary = ft_strdup(full_expansion);
		free(parameter);
		free(partial);
		free(expanded_param);
		temporary = find_expanding_index(temporary);
	}
	*expanded = full_expansion;
	return (1);
}


int	main(int argc, char *argv[], char **envp)
{
	char		*string;
	char		*expanded;

	string = "S$var2";
	expansion(string, &expanded, envp);
	printf("this should be the expanded string: %s\n", expanded);
	return (0);
}
