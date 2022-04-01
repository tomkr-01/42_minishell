#include "../inc/minishell.h"

static int	read_stdin_into_pipe(char *here_doc)
{
	int		status;
	int		pipe_ends[2];
	pid_t	process_id;

	status = pipe(pipe_ends);
	if (status == -1 || own_fork(&process_id) == -1)
		return (-1);
	if (process_id == 0)
	{
		close(pipe_ends[READ]);
		dup2(pipe_ends[WRITE], STDOUT_FILENO);
		close(pipe_ends[WRITE]);
		ft_putstr_fd(here_doc, 1);
		ft_atexit(1);
	}
	else if (process_id > 0)
	{
		wait(NULL);
		close(pipe_ends[WRITE]);
		dup2(pipe_ends[READ], STDIN_FILENO);
		close(pipe_ends[READ]);
	}
	return (status);
}

static int	count(const char *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i] != '\0' && s[i] == c)
		i++;
	while (s[i])
	{
		if (s[i] == c && s[i + 1] != c)
			count++;
		i++;
	}
	count += 1;
	return (count);
}

static int	is_ambiguous_redirect(t_redirection *redir, char **file,
			int *status)
{
	char	*filename_token;
	char	*expanded_string;

	filename_token = ft_strdup(redir->name);
	expanded_string = expander(filename_token, false);
	if (ft_strcmp(redir->name, expanded_string) != 0)
	{
		*file = ft_strtrim_free(expanded_string, " ");
		if (*file == NULL || *file[0] == '\0' || count(*file, ' ') > 0)
		{
			put_stderr(SHELL, NULL, redir->name,
				"ambiguous redirect");
		}
		else
			return (0);
		ft_free((void **)file);
		*status = 1;
		return (-1);
	}
	else
		*file = quote_remover(expanded_string);
	return (0);
}

static int	open_files(t_redirection *redir, int *status)
{
	int		fd;
	char	*file;

	file = NULL;
	if (is_ambiguous_redirect(redir, &file, status) == -1)
		return (-1);
	if (redir->type == IN)
		fd = open(file, O_RDONLY);
	else if (redir->type == OUT)
		fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	else if (redir->type == APPEND)
		fd = open(file, O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (fd < 0)
	{
		perror(file);
		ft_free((void **)&file);
		*status = 1;
		return (-1);
	}
	if (redir->type == IN)
		dup2(fd, STDIN_FILENO);
	else
		dup2(fd, STDOUT_FILENO);
	ft_free((void **)&file);
	return (0);
}

void	execute_redirections(t_redirection **redirections, int **pipe_ends,
			int *status)
{
	t_redirection	*redir;

	redir = *redirections;
	while (redir != NULL)
	{
		if (redir->type == HEREDOC)
			read_stdin_into_pipe(redir->name);
		else
		{
			if (open_files(redir, status) == -1)
			{
				if (pipe_ends != NULL)
				{
					free(pipe_ends);
					pipe_ends = NULL;
				}
				ft_atexit(*status);
			}
		}
		redir = redir->next;
	}
}
