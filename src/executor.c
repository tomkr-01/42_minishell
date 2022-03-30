#include "../inc/minishell.h"

extern t_minishell g_msh;

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
	int			status;

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

// void	ft_free_split(char **array)
// {
// 	int	i;

// 	i = 0;
// 	while (array && array[i])
// 	{
// 		free(array[i]);
// 		i++;
// 	}
// 	free(array);
// 	array = NULL;
// }

// void	ft_free_array2(char **arr)
// {
// 	size_t	i;

// 	if (arr == NULL)
// 		return ;
// 	i = 0;
// 	while (arr[i] != NULL)
// 	{
// 		free(arr[i]);
// 		arr[i] = NULL;
// 		i++;
// 	}
// 	if (arr != NULL)
// 		free(arr);
// 	arr = NULL;
// }

void	clear_table_row(t_table **table)
{
	t_redirection	*tmp;

	while ((*table)->redirections != NULL)
	{
		tmp = (*table)->redirections->next;
		ft_free((void **)&(*table)->redirections->name);
		(*table)->redirections = tmp;
	}
	// if ((*table)->arguments != NULL)
	// 	ft_free_array2((*table)->arguments);
	ft_free_array(&(*table)->arguments);
	(*table)->arguments = NULL;
	// if ((*table)->arguments != NULL)
	// {
	// 	free((*table)->arguments);
	// 	(*table)->arguments = NULL;
	// }
}

int	is_ambiguous_redirect(t_table **table, char **file, int *status)
{
	char	*filename_token;
	char	*expanded_string;

	filename_token = ft_strdup((*table)->redirections->name);
	expanded_string = expander(filename_token, false);
	if (ft_strcmp((*table)->redirections->name, expanded_string) != 0)
	{
		*file = ft_strtrim_free(expanded_string, " ");
		if (*file == NULL || *file[0] == '\0' || count(*file, ' ') > 1)
		{
			put_stderr(SHELL, NULL, (*table)->redirections->name,
				"ambiguous redirect");
			clear_table_row(table);
			// table_clear(table);
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

static void	send_null_to_stdin(void)
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
}

int	open_files(t_table **table, int *status)
{
	int			fd;
	char		*file;

	file = NULL;
	if (is_ambiguous_redirect(table, &file, status) == -1)
		return (-1);
	if ((*table)->redirections->type == IN)
		fd = open(file, O_RDONLY);
	else if ((*table)->redirections->type == OUT)
		fd = open(file, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	else if ((*table)->redirections->type == APPEND)
		fd = open(file, O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (fd < 0)
	{
		perror(file);
		ft_free((void **)&file);
		send_null_to_stdin();
		return ((*status = 1) * -1);
	}
	if ((*table)->redirections->type == IN)
		dup2(fd, STDIN_FILENO);
	else
		dup2(fd, STDOUT_FILENO);
	ft_free((void **)&file);
	return (0);
}

void	execute_child(t_table **table, int *status)
{
	char			*command;

	if ((*table)->arguments == NULL)
		exit(*status);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	change_attributes(true);
	if (check_builtins((*table)->arguments))
	{
		builtins((*table)->arguments);
		exit(g_msh.exit_code);
	}
	command = find_executable(ft_strdup((*table)->arguments[0]));
	execve(command, (*table)->arguments, g_msh.env);
	ft_free((void **)&command);
	if (access((*table)->arguments[0], F_OK) == 0 && access((*table)->arguments[0], X_OK) != 0)
	{
		put_stderr(SHELL, (*table)->arguments[0], NULL, "permission denied");
		// table_clear(table);
		exit(126);
	}
	else
	{
		put_stderr(SHELL, (*table)->arguments[0], NULL, "command not found");
		// table_clear(table);
		exit(127);
	}
}

int	read_stdin_into_pipe(char *here_doc)
{
	int			status;
	int			pipe_ends[2];
	pid_t		process_id;

	status = pipe(pipe_ends);
	if (status == -1)
		return (-1);
	if (own_fork(&process_id) == -1)
		return (-1);
	if (process_id == 0)
	{
		close(pipe_ends[READ]);
		dup2(pipe_ends[WRITE], STDOUT_FILENO);
		close(pipe_ends[WRITE]);
		ft_putstr_fd(here_doc, 1);
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

// void	execute_redirections(t_table **table, int *status)
// {
// 	t_redirection	**tmp_redir;

// 	tmp_redir = &(*table)->redirections;
// 	while (*tmp_redir != NULL)
// 	{
// 		if ((*tmp_redir)->type == HEREDOC)
// 			read_stdin_into_pipe((*tmp_redir)->name);
// 		else
// 			open_files(table, status);
// 		if (*tmp_redir != NULL)
// 			*tmp_redir = (*tmp_redir)->next;
// 	}
// }

int	execute_redirections(t_table **table, t_redirection *redir, int *status)
{
	while (redir != NULL)
	{
		if (redir->type == HEREDOC)
			read_stdin_into_pipe(redir->name);
		else
		{
			if (open_files(table, status) == -1)
				return (-1);
		}
		if (redir != NULL)
			redir = redir->next;
	}
	return (0);
}

void	child_process(t_table **table, int **pipe_ends, int *pipe_flag)
{
	int		redir_value;
	int		status;

	status = 0;
	if (*pipe_flag == 1)
		prepare_pipe(pipe_ends);
	redir_value = execute_redirections(table, (*table)->redirections, &status);
	if (redir_value == -1)
		ft_free_array(&(*table)->arguments);
	execute_child(table, &status);
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
	int		status;

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

/* signals catching not working for simple command */

void	simple_command(t_table *table)
{
	int		status;
	int		initial_stdin;
	int		initial_stdout;
	pid_t	process_id;
	
	status = 0;
	filestream_operations(&initial_stdin, &initial_stdout, 1);
	if (execute_redirections(&table, table->redirections, &status) == -1)
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

int	print_execution(t_table *table)
{
	if (table == NULL)
		return (-1);
	while (table != NULL)
	{
		while (table->redirections != NULL)
		{
			printf("%d: %s\n", table->redirections->type, table->redirections->name);
			table->redirections = table->redirections->next;
		}
		table = table->next;
	}
	return (1);
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

// void	executioner(t_table *table)
// {
// 	// print_execution(table);
// 	int		pipe_flag;
// 	int		initial_stdin;
// 	int		initial_stdout;
// 	int		*pipe_end;
// 	pid_t	process_id;

// 	if (initialize_pipe(&pipe_end) == -1)
// 		return ;
// 	filestream_operations(&initial_stdin, &initial_stdout, 1);
// 	if (table == NULL)
// 		return ;
// 	if (table->next == NULL)
// 		simple_command(table);
// 	else
// 	{
// 		while (table != NULL)
// 		{
// 			pipe_flag = pipe_found(&table, &pipe_end);
// 			if (pipe_flag == -1)
// 				return ;
// 			if (own_fork(&process_id) == -1)
// 				return ;
// 			if (process_id == 0)
// 				child_process(&table, &pipe_end, &pipe_flag);
// 			else if (process_id > 0)
// 				parent_process(&pipe_end, &pipe_flag);
// 			table = table->next;
// 		}
// 		wait_for_all(process_id, initial_stdin, initial_stdout);
// 	}
// }

// void	executioner(t_table *table)
// {
// 	int		pipe_flag;
// 	int		initial_stdin;
// 	int		initial_stdout;
// 	int		*pipe_end;
// 	pid_t	process_id;

// 	if (initialize_pipe(&pipe_end) == -1)
// 		return ;
// 	filestream_operations(&initial_stdin, &initial_stdout, 1);
// 	while (table != NULL)
// 	{
// 		pipe_flag = pipe_found(&table, &pipe_end);
// 		if (pipe_flag == -1)
// 			return ;
// 		if (check_builtins(table->arguments) && pipe_flag == 0)
// 		{
// 			execute_redirections(&table);
// 			builtins(table->arguments);
// 			dup2(initial_stdin, 0);
// 			dup2(initial_stdout, 1);
// 		}
// 		else
// 		{
// 			if (own_fork(&process_id) == -1)
// 				return ;
// 			if (process_id == 0)
// 				child_process(&table, &pipe_end, &pipe_flag);
// 			else if (process_id > 0)
// 				parent_process(&pipe_end, &pipe_flag);
// 		}
// 		table = table->next;
// 	}
// 	int		status = 0;
// 	waitpid(process_id, &status, 0);
// 	{
// 		if (WIFSIGNALED(status))
// 		{
// 			if (WTERMSIG(status) == 2)
// 				ft_putstr_fd("\n", 2);
// 			else if (WTERMSIG(status) == 3)
// 				ft_putstr_fd("Quit: 3\n", 2);
// 			g_msh.exit_code = 128 + WTERMSIG(status);
// 		}
// 		else if (WIFEXITED(status))
// 			g_msh.exit_code = WEXITSTATUS(status);
// 		dup2(initial_stdout, 1);
// 		dup2(initial_stdin, 0);
// 	}
// 	while (wait(NULL) != -1)
// 	{
// 		dup2(initial_stdout, 1);
// 		dup2(initial_stdin, 0);
// 	}
// }

// void	executor(t_table *table)
// {
// 	int		initial_stdin;
// 	int		initial_stdout;
// 	int		pipe_end[2];
// 	pid_t	process_id;

// 	initial_stdin = dup(STDIN_FILENO);
// 	initial_stdout = dup(STDOUT_FILENO);
// 	if (table != NULL && table->next == NULL)
// 		; // we have a simple command that doens't a pipe
// 	while (table != NULL)
// 	{
// 		if (table->next != NULL)
// 		{
// 			if (pipe(pipe_end) == -1)
// 				return ;
// 		}
// 		process_id = fork();
// 		if (process_id == -1)
// 			return ;
// 		else if (process_id == 0)
// 		{
// 			close(pipe_end[READ]);
// 			dup2(pipe_end[WRITE], STDOUT_FILENO);
// 			close(pipe_end[WRITE]);
// 			while (table->redirections != NULL)
// 				ft_open(table->redirections->type, table->redirections->name);
// 			table->arguments[0] = find_executable(table->arguments[0]);
// 			execve(table->arguments[0], table->arguments, g_msh.env);
// 			printf("command not found\n");
// 		}
// 		else if (process_id > 0)
// 		{
// 			close(pipe_end[WRITE]);
// 			dup2(pipe_end[READ], STDIN_FILENO);
// 			close(pipe_end[READ]);
// 		}
// 		table = table->next;
// 	}
// 	while (wait(NULL) != -1)
// 		continue ;
// }
