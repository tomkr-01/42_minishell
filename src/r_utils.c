#include "../inc/minishell.h"

char	**array_append_array(char **first, char **second)
{
	int		i;
	int		j;
	char	**new_array;

	i = -1;
	j = -1;
	new_array = (char **)malloc((ft_arrlen(first) + ft_arrlen(second) + 1)
			* sizeof(char *));
	if (new_array == NULL)
		return (NULL);
	while (first != NULL && first[++i] != NULL)
		new_array[i] = ft_strdup(first[i]);
	while (second != NULL && second[++j] != NULL)
		new_array[i + j] = ft_strdup(second[j]);
	new_array[i + j] = NULL;
	if (first != NULL)
		ft_free_array(&first);
	if (second != NULL);
		ft_free_array(&second);
	return (new_array);
}


char	*str_append_char(char *string, char c)
{
	int			index;
	int			str_len;
	char		*new;

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
	if (string != NULL)
		ft_free((void **)&string);
	return (new);
}

void	print_error_and_exit(char *command, char *message, int exit_code)
{
	write(2, "minishell: ", 11);
	write(2, command, ft_strlen(command));
	write(2, ": ",2);
	write(2, message, ft_strlen(message));
	write(2, "\n", 1);
	ft_free((void **)&command);
	exit(exit_code);
}

int	command_check(char *command)
{
	struct stat		statbuf;

	if (stat(command, &statbuf) == 0)
	{
		if (S_ISDIR(statbuf.st_mode))
			print_error_and_exit(command, "is a directory", 126);
		else if (ft_strncmp(command, "/", 1) != 0
			&& ft_strncmp(command, "./", 2) != 0)
			print_error_and_exit(command, "command not found", 127);
	}
	else
	{
		if (ft_strncmp(command, "./", 2) == 0)
			print_error_and_exit(command, "no such file or directory", 127);
	}
	return (0);
}

char	*find_executable(char *command)
{
	int		index;
	char	*executable;
	char	*absolute_path;
	char	**directories;

	command_check(command);
	if (access(command, F_OK | X_OK) == 0)
		return (command);
	index = 0;
	if (get_var("PATH") == NULL)
		return (command);
	directories = ft_split(get_var("PATH"), ':');
	// if (directories == NULL)
	// 	return (NULL);
	executable = ft_strjoin("/", command);
	while (directories[index] != NULL)
	{
		absolute_path = ft_strjoin(directories[index], executable);
		if (access(absolute_path, F_OK) == 0)
		{
			ft_free((void **)&executable);
			ft_free((void **)&directories);
			return (absolute_path);
		}
		ft_free((void **)&absolute_path);
		index++;
	}
	ft_free((void **)&executable);
	ft_free((void **)&directories);
	return (command);
}
