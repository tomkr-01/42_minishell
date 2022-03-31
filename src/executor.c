#include "../inc/minishell.h"

extern t_minishell	g_msh;

void	filestream_operations(int *initial_stdin, int *initial_stdout, int mode)
{
	if (mode == 1)
	{
		*initial_stdin = dup(STDIN_FILENO);
		*initial_stdout = dup(STDOUT_FILENO);
	}
	else if (mode == 2)
	{
		dup2(*initial_stdin, STDIN_FILENO);
		dup2(*initial_stdout, STDOUT_FILENO);
	}
	else if (mode == 3)
	{
		close(*initial_stdin);
		close(*initial_stdout);
	}
}

int	pipe_found(t_table **table, int **pipe_ends)
{
	int	status;

	status = 0;
	if ((*table)->next != NULL)
	{
		status = pipe(*pipe_ends);
		if (status == -1)
			return (-1);
		return (1);
	}
	return (status);
}

int	own_fork(pid_t *process_id)
{
	*process_id = fork();
	if (*process_id == -1)
	{
		perror("fork");
		return (-1);
	}
	return (0);
}

void	prepare_pipe(int **pipe_ends)
{
	close((*pipe_ends)[READ]);
	dup2((*pipe_ends)[WRITE], STDOUT_FILENO);
	close((*pipe_ends)[WRITE]);
}

void	clear_table_row(t_table **table)
{
	t_redirection	*tmp;

	while ((*table)->redirections != NULL)
	{
		tmp = (*table)->redirections->next;
		ft_free((void **)&(*table)->redirections->name);
		(*table)->redirections = tmp;
	}
	ft_free_array(&(*table)->arguments);
	(*table)->arguments = NULL;
}

int	is_ambiguous_redirect(t_redirection *redir, char **file, int *status)
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

static int	send_null_to_stdin(void)
{
	pid_t	process_id;
	int		pipe_end[2];
	char	*line;

	line = NULL;
	pipe(pipe_end);
	process_id = fork();
	if (process_id == 0)
	{
		close(pipe_end[READ]);
		ft_putstr_fd(line, pipe_end[WRITE]);
		close(pipe_end[WRITE]);
		exit(1);
	}
	else if (process_id > 0)
	{
		wait(NULL);
		close(pipe_end[WRITE]);
		dup2(pipe_end[READ], 0);
		close(pipe_end[READ]);
	}
	return (1);
}

int	open_files(t_redirection *redir, int *status)
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

void	set_attributes(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	change_attributes(true);
}

void	execute_child_errors(t_table **table)
{
	int	exit_status;

	exit_status = 0;
	if (access((*table)->arguments[0], F_OK) == 0
		&& access((*table)->arguments[0], X_OK) != 0)
	{
		put_stderr(SHELL, (*table)->arguments[0], NULL, "permission denied");
		exit_status = 126;
	}
	else
	{
		put_stderr(SHELL, (*table)->arguments[0], NULL, "command not found");
		exit_status = 127;
	}
	table_clear(&(get_head()->next));
	ft_free_array(&g_msh.env);
	exit(exit_status);
}

int	execute_child(t_table **table, int *status)
{
	char	*command;

	if ((*table)->arguments == NULL)
	{
		table_clear(&(get_head()->next));
		ft_free_array(&g_msh.env);
		exit(*status);
	}
	set_attributes();
	if (check_builtins((*table)->arguments))
	{
		builtins((*table)->arguments);
		table_clear(&(get_head()->next));
		ft_free_array(&g_msh.env);
		return (1);
	}
	command = find_executable(ft_strdup((*table)->arguments[0]));
	execve(command, (*table)->arguments, g_msh.env);
	ft_free((void **)&command);
	execute_child_errors(table);
	return (0);
}

int	read_stdin_into_pipe(char *here_doc)
{
	int		status;
	int		pipe_ends[2];
	pid_t	process_id;

	status = pipe(pipe_ends);
	if (status == -1 || own_fork(&process_id) == -1)
		return (-1);
	// if (own_fork(&process_id) == -1)
	// 	return (-1);
	if (process_id == 0)
	{
		close(pipe_ends[READ]);
		dup2(pipe_ends[WRITE], STDOUT_FILENO);
		close(pipe_ends[WRITE]);
		ft_putstr_fd(here_doc, 1);
		table_clear(&(get_head()->next));
		ft_free_array(&g_msh.env);
		exit(1);
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

int	execute_redirections(t_redirection **redirections, int *status)
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
				ft_free_array(&g_msh.env);
				return (-1);
			}
		}
		redir = redir->next;
	}
	return (0);
}

void	child_process(t_table **table, int **pipe_ends, int *pipe_flag)
{
	int	redir_value;
	int	status;

	status = 0;
	if (*pipe_flag == 1)
		prepare_pipe(pipe_ends);
	redir_value = execute_redirections(&(*table)->redirections, &status);
	if (redir_value == -1)
		ft_free_array(&(*table)->arguments);
	free(*pipe_ends);
	if (execute_child(table, &status) == 1)
		exit(g_msh.exit_code);
}

void	parent_process(int **pipe_ends, int *pipe_flag)
{
	if (*pipe_flag == 1)
	{
		close((*pipe_ends)[WRITE]);
		dup2((*pipe_ends)[READ], STDIN_FILENO);
		close((*pipe_ends)[READ]);
	}
}

int	initialize_pipe(int **pipe_ends)
{
	*pipe_ends = (int *)malloc(2 * sizeof(int));
	if (*pipe_ends == 0)
		return (-1);
	return (0);
}

void	wait_for_last(int pid, int initial_stdin, int initial_stdout)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			ft_putstr_fd("\n", 2);
		else if (WTERMSIG(status) == SIGQUIT)
			ft_putstr_fd("Quit: 3\n", 2);
		g_msh.exit_code = 128 + WTERMSIG(status);
	}
	else if (WIFEXITED(status))
		g_msh.exit_code = WEXITSTATUS(status);
	filestream_operations(&initial_stdin, &initial_stdout, 2);
}

void	wait_for_all(int pid, int initial_stdin, int initial_stdout)
{
	wait_for_last(pid, initial_stdin, initial_stdout);
	while (wait(NULL) != -1)
		filestream_operations(&initial_stdin, &initial_stdout, 2);
}

void	simple_command(t_table *table)
{
	int		status;
	int		initial_stdin;
	int		initial_stdout;
	pid_t	process_id;

	status = 0;
	filestream_operations(&initial_stdin, &initial_stdout, 1);
	if (execute_redirections(&table->redirections, &status) == -1)
		ft_free_array(&table->arguments);
	if (check_builtins(table->arguments))
	{
		builtins(table->arguments);
		filestream_operations(&initial_stdin, &initial_stdout, 2);
		return ;
	}
	else
	{
		if (own_fork(&process_id) == -1)
			return ;
		if (process_id == 0)
			execute_child(&table, &status);
	}
	wait_for_last(process_id, initial_stdin, initial_stdout);
	filestream_operations(&initial_stdin, &initial_stdout, 3);
}

void	execute_pipeline(t_table *table)
{
	int		initial_stdin;
	int		initial_stdout;
	int		pipe_flag;
	int		*pipe_ends;
	pid_t	process_id;

	if (initialize_pipe(&pipe_ends) == -1)
		return ;
	filestream_operations(&initial_stdin, &initial_stdout, 1);
	while (table != NULL)
	{
		pipe_flag = pipe_found(&table, &pipe_ends);
		if (pipe_flag == -1)
			return ;
		if (own_fork(&process_id) == -1)
			return ;
		if (process_id == 0)
			child_process(&table, &pipe_ends, &pipe_flag);
		else if (process_id > 0)
			parent_process(&pipe_ends, &pipe_flag);
		table = table->next;
	}
	ft_free((void **)&pipe_ends);
	wait_for_all(process_id, initial_stdin, initial_stdout);
	filestream_operations(&initial_stdin, &initial_stdout, 3);
}

void	executioner(t_table *table)
{
	signal(SIGINT, SIG_IGN);
	if (table == NULL)
		return ;
	if (table->next == NULL)
		simple_command(table);
	else
		execute_pipeline(table);
}
