#include "../inc/minishell.h"
#include <fcntl.h>

// void	heredoc(char *delimiter)
// {
// 	bool	expansion;
// 	char	*initial_delim;
// 	char	*delim;
// 	char	*line;

// 	expansion = false;
// 	initial_delim = ft_strdup(delimiter);
// 	if (initial_delim == NULL)
// 		return ;
// 	delim = quote_remover(initial_delim);
// 	if (ft_strcmp(delim, delimiter) == 0)
// 		expansion = true;
// 	free(initial_delim);
// 	while (1)
// 	{
// 		ft_putstr_fd("> ", 2);
// 		line = get_next_line(STDIN_FILENO);
// 		if (ft_strncmp(line, delimiter, ft_strlen(line)) == 0)
// 		{
// 			free(line);
// 			// close(STDIN_FILENO);
// 			break ;
// 		}
// 		if (expansion)
// 			line = expander(line);
// 		ft_putstr_fd(line, STDIN_FILENO);
// 		free(line);
// 	}
// 	// free(delim);
// }

// int	main(void)
// {
// 	int		fd;

// 	// fd = open("shit", O_CREAT | O_TRUNC | O_WRONLY, 0644);
// 	// if (fd < 0)
// 	// 	printf("no file made\n");
// 	// dup2(fd, 0);
// 	heredoc(ft_strdup("EOF"));
// 	return (0);
// }
