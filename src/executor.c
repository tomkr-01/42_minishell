#include "../inc/minishell.h"

extern t_minishell	g_msh;

void	copy_std_filestreams(int *initial_stdin, int *initial_stdout)
{
	*initial_stdin = dup(STDIN_FILENO);
	*initial_stdout = dup(STDOUT_FILENO);
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
		return (-1);
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
		free((*table)->redirections->name);
		(*table)->redirections->next = NULL;
		(*table)->redirections = tmp;
	}
	free((*table)->redirections);
	(*table)->redirections = NULL;
	if ((*table)->arguments != NULL)
		; // free split
}

int	is_ambiguous_redirect(t_table **table, char **file)
{
	bool	clear_list;
	char	*filename_token;
	char	*expanded_string;

	clear_list = false;
	filename_token = ft_strdup((*table)->redirections->name);
	expanded_string = expander(filename_token);
	// this check doesn't work as intended if we have the quotes removed
	// before it, it still should work but with more instructions than necessary
	if (ft_strcmp((*table)->redirections->name, expanded_string) != 0)
	{
		*file = ft_strtrim(expanded_string, " ");
		// free expanded string
		if (*file == NULL || *file[0] == '\0')
			clear_list = true;
		if (count(*file, ' ') > 1)
			clear_list = true;
		if (clear_list)
			clear_table_row(table);
		else
			return (0);
		write(2, "minishell: ambiguous redirect\n", 30);
		return (-1);
	}
	else
		*file = filename_token;
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
	char			*command;
	struct stat		*statbuf;

	command = NULL;
	if ((*table)->arguments != NULL)
		command = find_executable((*table)->arguments[0]);
	execve(command, (*table)->arguments, g_msh.env);
	if (!stat(command, statbuf))
	{
		if (!(statbuf->st_mode & 0111))
			write(2, "minishell: permission denied\n", 29);
	}
	// if (access(command, F_OK) == 0)
	// {
	// 	if (access(command, X_OK) != 0)
	// 		write(2, "minishell: permission denied\n", 29);
	// }
	else
		write(2, "minishell: command not found\n", 29);
	// free command
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
		close(0);
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

void	execute_redirections(t_table **table, int **pipe_ends, int *pipe_flag, int *initial_stdin)
{
	char	*here_string;

	here_string = NULL;
	if (*pipe_flag == 1)
		prepare_pipe(pipe_ends);
	while ((*table)->redirections != NULL)
	{
		if ((*table)->redirections->type == HEREDOC)
		{
			// here_string = heredoc((*table)->redirections->name, initial_stdin);
			// if (read_stdin_into_pipe(here_string) == -1)
			read_stdin_into_pipe((*table)->redirections->name);
			// 	exit(1);
		}
		else
			open_files(table);
		if ((*table)->redirections != NULL)
			(*table)->redirections = (*table)->redirections->next;
	}
}

void	child_process(t_table **table, int **pipe_ends, int *pipe_flag, int *initial_stdin)
{
	execute_redirections(table, pipe_ends, pipe_flag, initial_stdin);
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

// void	execute_bin(t_table **table, int **pipe_ends, int *pipe_flag)
// {
// 	pid_t		process_id;

// 	if (own_fork(&process_id) == -1)
// 		return ; // exit free maybe here
// 	if (process_id > 0)
// 		child_process(table, pipe_ends, pipe_flag);
// 	else if (process_id == 0)
// 		parent_process(pipe_ends, pipe_flag);
// }

void	executioner(t_table *table)
{
	int		pipe_flag;
	int		initial_stdin;
	int		initial_stdout;
	int		*pipe_end;
	pid_t	process_id;

	if (initialize_pipe(&pipe_end) == -1)
		return ;
	copy_std_filestreams(&initial_stdin, &initial_stdout);
	while (table != NULL)
	{
		pipe_flag = pipe_found(&table, &pipe_end);
		if (pipe_flag == -1)
			return ;
		/* 
		if (check_builtins)
			execute builtins
		else
			alles drunter hier rein
		*/
		if (own_fork(&process_id) == -1)
			return ;
		if (process_id == 0)
			child_process(&table, &pipe_end, &pipe_flag, &initial_stdin);
		else if (process_id > 0)
			parent_process(&pipe_end, &pipe_flag);
		table = table->next;
	}
	while (wait(NULL) != -1)
	{
		dup2(initial_stdout, 1);
		dup2(initial_stdin, 0);
	}
}

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
