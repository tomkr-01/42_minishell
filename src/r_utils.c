#include "../inc/minishell.h"

extern t_minishell		g_msh;

char	**array_append_array(char **first, char **second)
{
	int		i;

	i = 0;
	if (second == NULL)
		return (first);
	while (second != NULL && second[i] != NULL)
	{
		first = add_array_element(first, second[i]);
		i++;
	}
	ft_free_array(&second);
	return (first);
}

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

static void	print_error_and_exit(char *command, char *message, int exit_code, t_table *head)
{
	write(2, "minishell: ", 11);
	write(2, command, ft_strlen(command));
	write(2, ": ", 2);
	write(2, message, ft_strlen(message));
	write(2, "\n", 1);
	ft_free((void **)&command);
	table_clear(&head);
	ft_free_array(&g_msh.env);
	exit(exit_code);
}

static int	command_check(char *command, char *path, t_table *head)
{
	struct stat		statbuf;

	if (stat(command, &statbuf) != 0)
	{
		if (ft_strncmp(command, "./", 2) == 0
			|| ft_strncmp(command, "/", 1) == 0)
		{
			ft_free((void **)&path);
			print_error_and_exit(command, "no such file or directory", 127, head);
		}
		else if (path == NULL)
			print_error_and_exit(command, "no such file or directory", 127, head);
	}
	else
	{
		if (path != NULL && ft_strncmp(command, "./", 2) != 0
			&& ft_strncmp(command, "/", 1) != 0)
		{
			ft_free((void **)&path);
			print_error_and_exit(command, "command not found", 127, head);
		}
	}
	if (stat(command, &statbuf) == 0)
	{
		if (S_ISDIR(statbuf.st_mode))
		{
			ft_free((void **)&path);
			print_error_and_exit(command, "is a directory", 126, head);
		}
	}
	return (1);
}

static char	*search_in_directories(char	**directories, char *command,
	char *executable)
{
	int		index;
	char	*absolute_path;

	index = 0;
	while (directories[index] != NULL)
	{
		absolute_path = ft_strjoin(directories[index], executable);
		if (access(absolute_path, F_OK) == 0 && command != NULL)
		{
			ft_free((void **)&executable);
			ft_free_array(&directories);
			ft_free((void **)&command);
			return (absolute_path);
		}
		ft_free((void **)&absolute_path);
		index++;
	}
	ft_free((void **)&executable);
	ft_free_array(&directories);
	return (command);
}

char	*find_executable(char *command, t_table *head)
{
	char	*result;
	char	*path;
	char	*executable;
	char	**directories;

	path = get_var("PATH");
	command_check(command, path, head);
	if (access(command, F_OK) == 0)
	{
		ft_free((void **)&path);
		return (command);
	}
	directories = ft_split(path, ':');
	ft_free((void **)&path);
	if (directories == NULL)
		return (NULL);
	executable = ft_strjoin("/", command);
	result = search_in_directories(directories, command, executable);
	return (result);
}

// void	ft_free_array(char ***arr)
// {
// 	char	**parser;
// 	size_t	i;

// 	parser = *arr;
// 	i = 0;
// 	while (arr != NULL && parser != NULL && parser[i] != NULL)
// 	{
// 		ft_free((void **)&parser[i++]);
// 	}
// 	ft_free((void **)*arr);
// }

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
		p->arguments = NULL;
		redi_clear(&(p->redirections));
		ft_free((void **)&p);
		p = next;
	}
	*table = NULL;
}
