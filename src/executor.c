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

void	ft_free_split(char **array)
{
	int	i;

	i = 0;
	while (array && array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
	array = NULL;
}

void	clear_table_row(t_table **table)
{
	t_redirection	*tmp;

	while ((*table)->redirections != NULL)
	{
		tmp = (*table)->redirections->next;
		ft_free((void **)&(*table)->redirections->name);
		// (*table)->redirections->next = NULL;
		(*table)->redirections = tmp;
	}
	// ft_free((void **)&(*table)->redirections);
	// (*table)->redirections = NULL;
	// 
	// if ((*table)->arguments != NULL)
}

int	is_ambiguous_redirect(t_table **table, char **file)
{
	bool	clear_list;
	char	*filename_token;
	char	*expanded_string;

	clear_list = false;
	filename_token = ft_strdup((*table)->redirections->name);
	expanded_string = expander(filename_token, false);
	if (ft_strcmp((*table)->redirections->name, expanded_string) != 0)
	{
		*file = ft_strtrim(expanded_string, " ");
		// ft_free((void **)expanded_string);
		if (*file == NULL || *file[0] == '\0')
			clear_list = true;
		if (count(*file, ' ') > 1)
			clear_list = true;
		if (clear_list)
			clear_table_row(table);
		else
			return (0);
		write(2, "minishell: ambiguous redirect\n", 30);
		// write(2, (*table)->redirections->name, ft_strlen((*table)->redirections->name));
		return (-1);
	}
	else
		*file = quote_remover(expanded_string);
	return (0);
}

int	open_files(t_table **table)
{
	int			fd;
	int			status;
	char		*file;

	file = NULL;
	status = is_ambiguous_redirect(table, &file);
	if (status == -1)
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
		return (-1);
	}
	if ((*table)->redirections->type == IN)
		dup2(fd, STDIN_FILENO);
	else
		dup2(fd, STDOUT_FILENO);
	return (0);
}

void	execute_child(t_table **table)
{
	char			*copy;
	char			*command;

	command = NULL;
	if ((*table)->arguments == NULL)
		exit(0);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	change_attributes(true);
	copy = ft_strdup((*table)->arguments[0]);
	if (check_builtins((*table)->arguments))
	{
		builtins((*table)->arguments);
		exit(g_msh.exit_code);
	}
	if ((*table)->arguments != NULL)
		command = find_executable((*table)->arguments[0]);
	execve(command, (*table)->arguments, g_msh.env);
	write(2, command, ft_strlen(command));
	write(2, ": ", 2);
	if (access(command, F_OK) == 0)
	{
		if (access(command, X_OK) != 0)
			write(2, "minishell: permission denied\n", 29);
		exit(126);
	}
	else
		write(2, "minishell: command not found\n", 29);
	exit(127);
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

void	execute_redirections(t_table **table)
{
	while ((*table)->redirections != NULL)
	{
		if ((*table)->redirections->type == HEREDOC)
			read_stdin_into_pipe((*table)->redirections->name);
		else
			open_files(table);
		if ((*table)->redirections != NULL)
			(*table)->redirections = (*table)->redirections->next;
	}
}

void	child_process(t_table **table, int **pipe_ends, int *pipe_flag)
{
	if (*pipe_flag == 1)
		prepare_pipe(pipe_ends);
	execute_redirections(table);
	execute_child(table);
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

/* signals catching not working for simple command */
void	simple_command(t_table *table)
{
	int		initial_stdin;
	int		initial_stdout;
	pid_t	process_id;
	
	if (check_builtins(table->arguments))
	{
		execute_redirections(&table);
		builtins(table->arguments);
		filestream_operations(&initial_stdin, &initial_stdout, 2);
		return ;
	}
	else
	{
		if (own_fork(&process_id) == -1)
			return ;
		if (process_id == 0)
		{
			execute_redirections(&table);
			execute_child(&table);
		}
	}
	wait_for_last(process_id, initial_stdin, initial_stdout);
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

void	executioner(t_table *table)
{
	// print_execution(table);
	int		pipe_flag;
	int		initial_stdin;
	int		initial_stdout;
	int		*pipe_end;
	pid_t	process_id;

	if (initialize_pipe(&pipe_end) == -1)
		return ;
	filestream_operations(&initial_stdin, &initial_stdout, 1);
	if (table == NULL)
		return ;
	if (table->next == NULL)
		simple_command(table);
	else
	{
		while (table != NULL)
		{
			pipe_flag = pipe_found(&table, &pipe_end);
			if (pipe_flag == -1)
				return ;
			if (own_fork(&process_id) == -1)
				return ;
			if (process_id == 0)
				child_process(&table, &pipe_end, &pipe_flag);
			else if (process_id > 0)
				parent_process(&pipe_end, &pipe_flag);
			table = table->next;
		}
		wait_for_all(process_id, initial_stdin, initial_stdout);
	}
}

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
