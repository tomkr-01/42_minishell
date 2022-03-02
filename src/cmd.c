#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include "../inc/minishell.h"
#include <stdlib.h>

// // to show tom that we have to dup the old content of the array into the new one

// int	main(void)
// {
// 	int		index;
// 	int		j;
// 	char	**arr;
// 	char	**new;

// 	index = 0;
// 	arr = (char **)malloc(3 * sizeof(char *));
// 	if (arr == NULL)
// 		return (-1);
// 	while (index < 3)
// 	{
// 		arr[index] = (char *)malloc(10 * sizeof(char));
// 		if (arr[index] == NULL)
// 			return (-2);
// 		index++;
// 	}
// 	index = 0;
// 	while (index < 2)
// 	{
// 		j = 0;
// 		while (j < 9)
// 		{
// 			arr[index][j] = 'a';
// 			j++;
// 		}
// 		arr[index][j] = '\0';
// 		// printf("index: %i, %s\n", index, arr[index]);
// 		index++;
// 	}
// 	arr[index] = NULL;
// 	new = (char **)malloc(4 * sizeof(char *));
// 	if (new == NULL)
// 		return (-3);
// 	index = 0;
// 	while (index < 2)
// 	{
// 		new[index] = ft_strdup(arr[index]);
// 		// printf("new index: %i, %s\n", index, new[index]);

// 		// free(arr[index]);
// 		// arr[index] = NULL;
// 		index++;
// 	}
// 	free(arr);
// 	arr = NULL;
// 	new[index] = (char *)malloc(5 * sizeof(char));
// 	if (new[index] == NULL)
// 		return (-4);
// 	j = 0;
// 	while (j < 4)
// 	{
// 		new[index][j] = 'b';
// 		j++;
// 	}
// 	new[index][j] = '\0';
// 	index++;
// 	new[index] = NULL;

// 	index = 0;
// 	while (new[index] != NULL)
// 	{
// 		printf("it works: %s\n", new[index]);
// 		index++;
// 	}
// 	printf("it works: %s\n", new[index]);
// 	return (0);
// }

/* possible simple struct for the parser to put information into
typedef struct l_parser {
	int					type_flag;
	char				*string;
	struct l_parser		*next;
}
*/

int	open_in(char *file_name)
{
	int		fd;

	fd = open(file_name, O_RDONLY);
	if (fd < 0)
	{
		perror(file_name);
		return (-1);
	}
	dup2(fd, 0);
	return (0);
}

int	heredoc(char *delimiter)
{
	char	*line;

	while (1)
	{
		line = get_next_line(0);
		if (ft_strncmp(line, delimiter, ft_strlen(line)) == 0)
		{
			free(line);
			break ;
		}
		else
			ft_putstr_fd(line, 0);
		free(line);
	}
	return (0);
}

int	open_out(char *file_name)
{
	int		fd;

	fd = open(file_name, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
	{
		perror(file_name);
		return (-1);
	}
	dup2(fd, 1);
	return (0);
}

int	open_append(char *file_name)
{
	int		fd;

	fd = open(file_name, O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (fd < 0)
	{
		perror(file_name);
		return (-1);
	}
	dup2(fd, 1);
	return (0);
}

char	**add_array_element(char **old_arr, char *new_el)
{
	size_t	i;
	size_t	arr_size;
	char	**new_arr;

	i = 0;
	arr_size = ft_arrlen(old_arr);
	new_arr = (char **)malloc((arr_size + 2) * sizeof(char *));
	if (new_arr == NULL)
	{
		perror(new_el);
		exit(0);
	}
	while (i < arr_size)
	{
		new_arr[i] = ft_strdup(old_arr[i]);
		// free(old_arr[i]);
		// old_arr[i] = NULL;
		i++;
	}
	if (old_arr != NULL)
		free(old_arr);
	old_arr = NULL;
	new_arr[i] = ft_strdup(new_el);
	new_arr[i + 1] = NULL;
	return (new_arr);
}

char	*find_executable(char *command);

void	execution(t_list *token, char **envp)
{
	int			pip[2];
	int			pid;
	size_t		token_len;
	char		**command;
	int			std_fd[2];

	std_fd[0] = dup(STDIN_FILENO);
	std_fd[1] = dup(STDOUT_FILENO);
	command = NULL;
	while (token != NULL)
	{
		// pipe(pip);
		// token_len = ft_strlen(token->content);
		// dup2(std_fd[0], 0);
		// dup2(std_fd[1], 1);
		while (token != NULL && token->content[0] != '|')
		{
			token_len = ft_strlen(token->content);
			if (ft_strchr(token->content, '<') != NULL)
			{
				if (token_len == 1)
				{
					token = token->next;
					open_in(token->content);
				}
				else if (token_len == 2)
				{
					token = token->next;
					heredoc(token->content);
				}
			}
			else if (ft_strchr(token->content, '>') != NULL)
			{
				if (token_len == 1)
				{
					token = token->next;
					open_out(token->content);
				}
				else if (token_len == 2)
				{
					token = token->next;
					open_append(token->content);
				}
			}
			else
				command = add_array_element(command, token->content);
			token = token->next;
		}
		if (token != NULL && token->content[0] == '|')
			pipe(pip);
		if (token != NULL)
			token = token->next;
		pid = fork();
		if (pid == 0)
		{
			close(pip[0]);
			dup2(pip[1], 1);
			command[0] = find_executable(command[0]);
			execve(command[0], command, envp);
			printf("command not found\n");
		}
		else if (pid > 0)
		{
			close(pip[1]);
			dup2(pip[0], 0);
			close(0);
			dup2(std_fd[0], 0);
			dup2(std_fd[1], 1);
			// close(0);
		}
		free(command);
		command = NULL;
	}
	while (wait(NULL) != -1)
		continue ;
}

char	*find_executable(char *command)
{
	int		index;
	char	*path;
	char	*executable;
	char	*absolute_path;
	char	**directories;

	if (access(command, F_OK | X_OK) == 0)
		return (command);
	// code a ft_getenv function that does the same as getenv.
	path = getenv("PATH");
	if (path == NULL)
		return (command);
	directories = ft_split(path, ':');
	if (directories == NULL)
		return (command);
	index = 0;
	executable = ft_strjoin("/", command);
	while (directories[index] != NULL)
	{
		absolute_path = ft_strjoin(directories[index], executable);
		if (access(absolute_path, F_OK | X_OK) == 0)
		{
			free(command);
			free(executable);
			// free directories
			return (absolute_path);
		}
		free(absolute_path);
		index++;
	}
	free(executable);
	// free directories
	return (command);
}

int	main(int argc, char *argv[], char **envp)
{
	char	*exe;
	char	*command[2];
	t_list	*tokens;

	tokens = (t_list *)lexer("ls < cmd.c > out -l |  wc -l");
	if (tokens == NULL)
		return (0);
	if (syntax_check(tokens))
	{
		execution(tokens, envp);
	}
	return (0);
}
