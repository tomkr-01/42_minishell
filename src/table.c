#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/minishell.h"

extern t_minishell	g_msh;

t_table	*create_table_row()
{
	t_table		*row;

	row = (t_table *)malloc(sizeof(t_table));
	if (row == NULL)
		return (NULL);
	row->arguments = NULL;
	row->redirections = NULL;
	row->next = NULL;
	return (row);
}

t_redirection	*create_redirection(int type, char *name)
{
	t_redirection	*new_redirection;

	new_redirection = (t_redirection *)malloc(sizeof(t_redirection));
	if (new_redirection == NULL)
		return (NULL);
	new_redirection->type = type;
	new_redirection->name = name;
	new_redirection->next = NULL;
	return (new_redirection);
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

char	*heredoc_get_next_line(char **limiter)
{
	char	*line;
	char	*here_string;

	// call signalhandler here
	*limiter = ft_strjoin(*limiter, "\n");
	here_string = ft_strdup("");
	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (line == NULL || ft_strcmp(line, *limiter) == 0)
		{
			// maybe this causes error because you free NULL;
			free(line);
			send_null_to_stdin();
			break ;
		}
		here_string = ft_strjoin(here_string, line);
		free(line);
	}
	// maybe free limiter;
	return (here_string);
}

char	*heredoc_readline(char **limiter)
{
	char	*line;
	char	*here_string;

	// call signalhandler here
	*limiter = ft_strjoin(*limiter, "\n");
	here_string = ft_strdup("");
	while (1)
	{
		line = readline("> ");
		line = ft_strjoin(line, "\n");
		if (line == NULL || ft_strcmp(line, *limiter) == 0)
		{
			free(line);
			send_null_to_stdin();
			break ;
		}
		here_string = ft_strjoin(here_string, line);
		free(line);
	}
	return (here_string);
}

void	heredoc(char **token_content)
{
	int			fd;
	bool		expansion;
	char		*delimiter_copy;
	char		*limiter;
	char		*line;
	char		*here_string;

	fd = dup(STDIN_FILENO);
	expansion = false;
	delimiter_copy = ft_strdup(*token_content);
	limiter = quote_remover(*token_content);
	if (ft_strcmp(delimiter_copy, limiter) != 0)
		expansion = true;
	if (isatty(STDIN_FILENO))
		*token_content = heredoc_readline(&limiter);
	else
		*token_content = heredoc_get_next_line(&limiter);
	free(delimiter_copy);
	free(limiter);
	dup2(fd, STDIN_FILENO);
	close(fd);
	if (expansion)
		*token_content = expander(*token_content);
}

void	append_redirection(t_table **lst, t_list *token, int redir_type)
{
	t_table			*temporary;
	t_redirection	*new_redirection;

	temporary = *lst;
	if (redir_type == HEREDOC)
		heredoc(&token->content);
	new_redirection = create_redirection(redir_type, token->content);
	if (temporary->redirections == NULL)
	{
		temporary->redirections = new_redirection;
		return ;
	}
	if (new_redirection == NULL)
		return ;
	while (temporary->redirections->next != NULL)
		temporary->redirections = temporary->redirections->next;
	new_redirection->next = temporary->redirections->next;
	temporary->redirections->next = new_redirection;
}

int	find_redirection_type(t_list **token, int *type)
{
	if (ft_strcmp((*token)->content, ">") == 0)
		*type = OUT;
	else if (ft_strcmp((*token)->content, "<") == 0)
		*type = IN;
	else if (ft_strcmp((*token)->content, ">>") == 0)
		*type = APPEND;
	else if (ft_strcmp((*token)->content, "<<") == 0)
		*type = HEREDOC;
	if (*type != 0)
		*token = (*token)->next;
	return (*type);
}

int	count(const char *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (s == NULL)
		return (0);
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

void	parse_command(t_list **token, t_table **table)
{
	char	*expanded_string;
	char	**argument_list;
	/* steps to consider inside the function
		1. save the unhandled token inside a variable
		2. call the variable expansion on the variable
			inside single quotes don't expand
			no variable replaced with NULL
			else expanded normally
		3. remove the quotes
		4. call the same function as for the split to count
		the word inside the string
		5. if bigger than 1 call function;
			array_append_array(char **first, char **second);
		   if not than just use add_array_element(char **old_arr, char *new);
	*/
	expanded_string = expander((*token)->content);
	if (expanded_string == NULL || expanded_string[0] == '\0')
		;
	else if (count(expanded_string, ' ') > 1)
	{	
		argument_list = ft_split(expanded_string, ' ');
		(*table)->arguments = array_append_array((*table)->arguments, argument_list);
		// free argument list
	}
	else if (count(expanded_string, ' ') == 1)
		(*table)->arguments = add_array_element((*table)->arguments, expanded_string);
	return ;
}

t_table	*parser(t_list *token)
{
	int				redir_type;
	t_table			*head;
	t_table			*table;
	t_redirection	*new_redirection;

	table = create_table_row();
	if (table == NULL)
		return (NULL);
	head = table;
	while (token != NULL)
	{
		redir_type = 0;
		if (ft_strcmp(token->content, PIPE) == 0)
		{
			table->next = create_table_row();
			table = table->next;
		}
		else if (find_redirection_type(&token, &redir_type) > 0)
			append_redirection(&table, token, redir_type);
		else
			parse_command(&token, &table);
		token = token->next;
	}
	return (head);
}

//////////////////////////////////////////////////////

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
	// if (!stat(command, statbuf))
	// {
	// 	if (!(statbuf->st_mode & 0111))
	// 		write(2, "minishell: permission denied\n", 29);
	// }
	if (access(command, F_OK) == 0)
	{
		if (access(command, X_OK) != 0)
			write(2, "minishell: permission denied\n", 29);
	}
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
		/* a builtin is executed in the parent process. a builtin in a pipeline is executed in a child process. a non builtin is always executed in a child process
		if (is_builtin = true)
			if (pipe_was_found = true)
				fork and execute
			else
				execute
		*/
		if (own_fork(&process_id) == -1)
			return ;
		if (process_id > 0)
			child_process(&table, &pipe_end, &pipe_flag, &initial_stdin);
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

///////////////////////////////////////////////

int	main(int argc, char *argv[], char **envp)
{
	char	*exe;
	char	*command[2];
	t_list	*tokens;
	t_table	*table;
	environment_init(envp);
	tokens = (t_list *)lexer("cat << EOF");
	if (tokens == NULL)
		return (0);
	if (syntax_check(tokens))
	{
		table = parser(tokens);
		// print_execution(table);
		executioner(table);
		// execution(tokens, envp);
	}
	return (0);
}

// gcc lexer.c syntax_check.c cmd.c ../libs/libft/libft.a -lreadline
// gcc lexer.c cmd.c ../libs/libft/libft.a -lreadline
// gcc lexer.c ast.c ../libs/libft/libft.a -lreadline

// ../libs/libft/libft.a -I/Users/rjasari/.brew/opt/readline/include -lreadline -L/Users/rjasari/.brew/opt/readline/lib