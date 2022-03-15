#include "../inc/minishell.h"

// char	**array_append_array(char **first, char **second)
// {
// 	int		i;
// 	int		j;
// 	char	**new_array;

// 	i = 0;
// 	j = 0;
// 	new_array = (char **)malloc((ft_arrlen(first) + ft_arrlen(second) + 1)
// 			* sizeof(char *));
// 	if (new_array == NULL)
// 		return (NULL);
// 	while (first[i] != NULL)
// 		new_array[i] = ft_strdup(first[i]);
// 	while (second[i] != NULL)
// 		new_array[i + j] = ft_strdup(second[j]);
// 	new_array[i + j] = NULL;
// 	/* to possibilities
// 		but check before if they are NULL
// 		1. free while copying
// 		2. call ft_free_split onto the old arrays
// 	*/
// 	return (new_array);
// }

// /* own get_env function */

// char	*ft_getenv(const char *variable, char **envp)
// {
// 	int		index;
// 	int		variable_len;
// 	char	*environment_variable;

// 	index = 0;
// 	environment_variable = ft_strjoin(variable, "=");
// 	variable_len = ft_strlen(environment_variable);
// 	while (envp[index])
// 	{
// 		if (ft_strncmp(envp[index], environment_variable, variable_len) == 0)
// 		{
// 			free(environment_variable);
// 			environment_variable = NULL;
// 			return (envp[index] + variable_len);
// 		}
// 		index++;
// 	}
// 	free(environment_variable);
// 	environment_variable = NULL;
// 	return (NULL);
// }

// size_t	protected_strlen(char *s)
// {
// 	int		index;

// 	index = 0;
// 	if (s == NULL)
// 		return (0);
// 	while (s[index] != '\0')
// 		index++;
// 	return (index);
// }

/* int array storing the beginning and ending of quotes? */

int	valid_expansion_character(int c)
{
	if (ft_isalnum(c) || c == '-' || c == '_')
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
		return (ft_strdup(&c));
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
