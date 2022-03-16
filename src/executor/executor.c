#include "../../inc/minishell.h"
#include "../parser/table.c"

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

char	*read_a_line()
{
	char	*line;

	if (isatty(STDIN_FILENO))
	{
		line = readline("> ");
		line = ft_strjoin(line, "\n");
	}
	else
	{
		write(2, "> ", 2);
		line = get_next_line(STDIN_FILENO);
	}
	return (line);
}

char	*heredoc(char *delim)
{
	char	*delimiter;
	char	*line;
	char	*here_string;

	delimiter = ft_strjoin(delim, "\n");
	here_string = ft_strdup("");
	while (1)
	{
		line = read_a_line();
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			send_null_to_stdin();
			break ;
		}
		here_string = ft_strjoin(here_string, line);
		free(line);
	}
	free(delimiter);
	return (here_string);
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

void	child_process(t_table **table, int **pipe_ends, int *pipe_flag)
{
	char		*here_doc;

	here_doc = NULL;
	if (*pipe_flag == 1)
		prepare_pipe(pipe_ends);
	while ((*table)->redirections != NULL)
	{
		if ((*table)->redirections->type == HEREDOC)
		{
			here_doc = heredoc((*table)->redirections->name);
			if (read_stdin_into_pipe(here_doc) == -1)
				exit(1);
		}
		// the heredoc takes the string unexpanded as the delimiter
		// if the delimiter is quoted, parameter expansion is turned off
		// else we need to read one line at a time and somehow dup
		// or write it into the stdin
		else
			open_files(table);
		// when opening the files first expand the filename token
		// and then run the ft_strtrim function on it
		// if after it we have either a NULL, '\0' or can find a space 
		// we error for ambigious redirect and then clear the whole table row
		if ((*table)->redirections != NULL)
			(*table)->redirections = (*table)->redirections->next;
	}
	execute_child(table);
	// checks if we have arguments and executes the file if ok
	// else checks for execution rights and return error message
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
		if (own_fork(&process_id) == -1)
			return ;
		if (process_id > 0)
			child_process(&table, &pipe_end, &pipe_flag);
		else if (process_id == 0)
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
