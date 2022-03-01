#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include "../inc/minishell.h"
#include <stdlib.h>

// to show tom that we have to dup the old content of the array into the new one

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

// char	*find_executable(char *command)
// {
// 	int		index;
// 	char	*path;
// 	char	*executable;
// 	char	*absolute_path;
// 	char	**directories;

// 	if (access(command, F_OK | X_OK) == 0)
// 		return (command);
// 	path = getenv("PATH");
// 	if (path == NULL)
// 		return (command);
// 	directories = ft_split(path, ':');
// 	if (directories == NULL)
// 		return (command);
// 	index = 0;
// 	executable = ft_strjoin("/", command);
// 	while (directories[index] != NULL)
// 	{
// 		absolute_path = ft_strjoin(directories[index], executable);
// 		if (access(absolute_path, F_OK | X_OK) == 0)
// 		{
// 			free(command);
// 			free(executable);
// 			// free directories
// 			return (absolute_path);
// 		}
// 		free(absolute_path);
// 		index++;
// 	}
// 	free(executable);
// 	// free directories
// 	return (command);
// }

void	execution(t_list *token)
{
	size_t		token_len;
	char		**command;

	while (token != NULL)
	{
		token_len = ft_strlen(token->content);
		while (token_len != 1 && token->content[0] != '|')
		{
			token_len = ft_strlen(token->content);
			if (ft_strchr(token->content, '<') != NULL)
			{
				if (token_len == 1)
					; // open infile into stdin
				else if (token_len == 2)
					; // call a heredoc
			}
			else if (ft_strchr(token->content, '>') != NULL)
			{
				if (token_len == 1)
					; // open a truncated file
				else if (token_len == 2)
					; // open in append mode
			}
		}
	}
}


// int	main(void)
// {
// 	char	*command;
// 	char	*path;
// 	char	**directories;

// 	path = getenv("PATH");
// 	printf("%s\n", path);
// 	return (0);
// }
