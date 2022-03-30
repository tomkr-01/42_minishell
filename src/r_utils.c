#include "../inc/minishell.h"

// char	**array_append_array(char **first, char **second)
// {
// 	int		i;
// 	int		j;
// 	char	**new_array;

// 	i = 0;
// 	j = -1;
// 	new_array = (char **)malloc((ft_arrlen(first) + ft_arrlen(second) + 1)
// 			* sizeof(char *));
// 	if (new_array == NULL)
// 		return (NULL);
// 	while (first != NULL && first[i] != NULL)
// 	{
// 		new_array[i] = ft_strdup(first[i]);
// 		i++;
// 	}
// 	while (second != NULL && second[++j] != NULL)
// 		new_array[i + j] = ft_strdup(second[j]);
// 	new_array[i + j] = NULL;
// 	ft_free_array(&first);
// 	ft_free_array(&second);
// 	return (new_array);
// }


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

// void	print_error_and_exit(char *command, char *message, int exit_code)
// {
// 	write(2, "minishell: ", 11);
// 	write(2, command, ft_strlen(command));
// 	write(2, ": ",2);
// 	write(2, message, ft_strlen(message));
// 	write(2, "\n", 1);
// 	ft_free((void **)&command);
// 	exit(exit_code);
// }

// int	command_check(char *command)
// {
// 	int				status;
// 	struct stat		*statbuf;

// 	status = 0;
// 	statbuf = (struct stat *)malloc(sizeof(struct stat));
// 	if (statbuf == NULL)
// 		return (-1);
// 	if (stat(command, statbuf) == 0)
// 	{
// 		if (S_ISDIR(statbuf->st_mode))
// 			print_error_and_exit(command, "is a directory", 126);
// 		else if (ft_strncmp(command, "/", 1) != 0
// 			&& ft_strncmp(command, "./", 2) != 0)
// 			print_error_and_exit(command, "command not found", 127);
// 	}
// 	else
// 	{
// 		if (ft_strncmp(command, "./", 2) == 0)
// 			print_error_and_exit(command, "no such file or directory", 127);
// 	}
// 	return (0);
// }

// char	*find_executable(char *command)
// {
// 	int		index;
// 	char	*path;
// 	char	*executable;
// 	char	*absolute_path;
// 	char	**directories;

// 	command_check(command);
// 	if (access(command, F_OK) == 0)
// 		return (command);
// 	index = 0;
// 	path = get_var("PATH");
// 	if (path == NULL)
// 		return (command);
// 	directories = ft_split(path, ':');
// 	ft_free((void **)&path);
// 	if (directories == NULL)
// 		return (NULL);
// 	executable = ft_strjoin("/", command);
// 	while (directories[index] != NULL)
// 	{
// 		absolute_path = ft_strjoin(directories[index], executable);
// 		if (access(absolute_path, F_OK) == 0)
// 		{
// 			ft_free((void **)&executable);
// 			ft_free((void **)&directories);
// 			return (absolute_path);
// 		}
// 		ft_free((void **)&absolute_path);
// 		index++;
// 	}
// 	ft_free((void **)&executable);
// 	ft_free((void **)&directories);
// 	return (command);
// }

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

int	command_check(char *command, char *path)
{
	struct stat		statbuf;

	if (stat(command, &statbuf) != 0)
	{
		if (ft_strncmp(command, "./", 2) == 0 || ft_strncmp(command, "/", 1) == 0)
			print_error_and_exit(command, "no such file or directory", 127);
		else if (path == NULL)
			print_error_and_exit(command, "no such file or directory", 127);
	}
	else
	{
		if (path != NULL && ft_strncmp(command, "./", 2) != 0 && ft_strncmp(command, "/", 1) != 0)
			print_error_and_exit(command, "command not found", 127);
	}
	if (stat(command, &statbuf) == 0)
	{
		if (S_ISDIR(statbuf.st_mode))
			print_error_and_exit(command, "is a directory", 126);
	}
	return (1);
}

char	*find_executable(char *command)
{
	int		index;
	char	*path;
	char	*executable;
	char	*absolute_path;
	char	**directories;

	index = 0;
	path = get_var("PATH");
	command_check(command, path);
	if (access(command, F_OK) == 0)
		return (ft_strdup(command));
	directories = ft_split(path, ':');
	ft_free((void **)&path);
	if (directories == NULL)
		return (NULL);
	executable = ft_strjoin("/", command);
	while (directories[index] != NULL)
	{
		absolute_path = ft_strjoin(directories[index], executable);
		if (access(absolute_path, F_OK) == 0 && command != NULL)
		{
			ft_free((void **)&executable);
			ft_free_array(&directories);
			return (absolute_path);
		}
		ft_free((void **)&absolute_path);
		index++;
	}
	ft_free((void **)&executable);
	ft_free_array(&directories);
	return (command);
}

void	redi_clear(t_redirection **redi)
{
	t_redirection	*p;
	t_redirection	*next;

	p = *redi;
	if (p == NULL)
		return ;
	while (p != NULL)
	{
		next = p->next;
		ft_free((void **)&(p->name));
		ft_free((void **)&p);
		p = next;
	}
	*redi = NULL;
}

void	table_clear(t_table **table)
{
	t_table	*p;
	t_table	*next;

	p = *table;
	if (p == NULL)
		return ;
	while (p != NULL)
	{
		next = p->next;
		ft_free_array(&(p->arguments));
		redi_clear(&(p->redirections));
		ft_free((void **)&p);
		p = next;
	}
	*table = NULL;
}
