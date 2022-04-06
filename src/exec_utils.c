#include "../inc/minishell.h"

extern t_minishell		g_msh;

char	*ft_chrdup(char c)
{
	char	*duplicate;

	duplicate = (char *)malloc(2 * sizeof(char));
	if (duplicate == NULL)
		return (NULL);
	duplicate[0] = c;
	duplicate[1] = '\0';
	return (duplicate);
}

char	*str_append_char(char *string, char c)
{
	int		index;
	int		str_len;
	char	*new;

	if (string == NULL)
		ft_chrdup(c);
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
	ft_free((void **)&string);
	return (new);
}

char	*search_in_directories(char	**directories, char *command, int *status)
{
	int		index;
	char	*executable;
	char	*absolute_path;

	index = 0;
	executable = ft_strjoin("/", command);
	while (directories[index] != NULL)
	{
		absolute_path = ft_strjoin(directories[index], executable);
		if (access(absolute_path, F_OK) == 0 && (command != NULL
				&& ft_strcmp(command, ".") != 0
				&& ft_strcmp(command, "..") != 0))
		{
			ft_free((void **)&executable);
			ft_free((void **)&command);
			return (absolute_path);
		}
		ft_free((void **)&absolute_path);
		index++;
	}
	*status = 1;
	ft_free((void **)&executable);
	return (command);
}

char	*find_executable(char *command)
{
	int		status;
	char	*path;
	char	**directories;

	status = 0;
	path = get_var("PATH");
	directories = ft_split(path, ':');
	if (!ft_strchr(command, '/') && path != NULL)
		command = search_in_directories(directories, command, &status);
	ft_free_array(&directories);
	ft_free((void **)&path);
	if (status == 1)
	{
		put_stderr(SHELL, command, NULL, "command not found");
		ft_free((void **)&command);
		ft_atexit(127);
	}
	return (command);
}
