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

// static void	send_null_to_stdin(void)
// {
// 	pid_t	process_id;
// 	int		pipe_end[2];
// 	char	*line;

// 	line = NULL;
// 	pipe(pipe_end);
// 	process_id = fork();
// 	if (process_id == 0)
// 	{
// 		close(pipe_end[READ]);
// 		ft_putstr_fd(line, pipe_end[WRITE]);
// 		close(pipe_end[WRITE]);
// 		exit(1);
// 	}
// 	else if (process_id > 0)
// 	{
// 		wait(NULL);
// 		close(pipe_end[WRITE]);
// 		dup2(pipe_end[READ], 0);
// 		close(pipe_end[READ]);
// 	}
// }

void	heredoc_signals(int sig)
{
	if (sig == SIGINT)
	{
		close(STDIN_FILENO);
		write(2, "\n", 1);
	}
}

char	*heredoc_get_next_line(char **limiter)
{
	char	*line;
	char	*here_string;

	signal(SIGINT, &heredoc_signals);
	*limiter = ft_strjoin(*limiter, "\n");
	here_string = ft_strdup("");
	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (line == NULL || ft_strcmp(line, *limiter) == 0)
		{
			ft_free((void **)&line);
			// send_null_to_stdin();
			break ;
		}
		here_string = ft_strjoin(here_string, line);
		if (here_string == NULL)
			break ;
		ft_free((void **)&line);
	}
	return (here_string);
}

char	*heredoc_readline(char **limiter)
{
	char	*line;
	char	*here_string;
	// signal(SIGINT, &heredoc_signals);

	signal(SIGINT, &heredoc_signals);
	here_string = NULL;
	here_string = ft_strdup("");
	while (1)
	{
		line = readline("> ");
		if (line == NULL || ft_strcmp(line, *limiter) == 0)
		{
			ft_free((void **)&line);
			// send_null_to_stdin();
			break ;
		}
		here_string = ft_strjoin(here_string, ft_strjoin(line, "\n"));;
		if (here_string == NULL)
			break ;
		ft_free((void **)&line);
	}
	return (here_string);
}

void	heredoc(char **token_content)
{
	int			fd;
	bool		expansion;
	char		*delimiter_copy;
	char		*limiter;

	change_attributes(false);
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
	if (close(STDIN_FILENO) == -1)
		*token_content = NULL;
	if (dup2(fd, STDIN_FILENO) == -1)
		close(fd);
	// dup2(fd, STDIN_FILENO);
	close(fd);
	if (expansion)
		*token_content = expander(*token_content, true);
}

int	append_redirection(t_table **lst, t_list *token, int redir_type)
{
	t_table			*temporary;
	t_redirection	*new_redirection;

	temporary = *lst;
	if (redir_type == HEREDOC)
	{
		heredoc(&token->content);
		if (token->content == NULL)
			return (-1);
	}
	new_redirection = create_redirection(redir_type, token->content);
	if (temporary->redirections == NULL)
	{
		temporary->redirections = new_redirection;
		return (0);
	}
	if (new_redirection == NULL)
		return (-1);
	while (temporary->redirections->next != NULL)
		temporary->redirections = temporary->redirections->next;
	new_redirection->next = temporary->redirections->next;
	temporary->redirections->next = new_redirection;
	return (0);
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

void	print_array_of_arrays(char **array)
{
	int		index;

	if (array == NULL)
		return ;
	index = 0;
	while (array[index] != NULL)
		printf("%s\n", array[index++]);
	return ;
}

void	parse_command(t_list **token, t_table **table)
{
	char	*expanded_string;
	char	**argument_list;

	argument_list = NULL;
	expanded_string = expander((*token)->content, true);
	if (((*table)->arguments != NULL && check_builtins((*table)->arguments))
			|| count(expanded_string, ' ') == 1)
		(*table)->arguments = add_array_element((*table)->arguments, expanded_string);
	else if (count(expanded_string, ' ') > 1)
	{
		argument_list = ft_split(expanded_string, ' ');
		(*table)->arguments = array_append_array((*table)->arguments, argument_list);
		ft_free_array(&argument_list);
	}
	return ;
}

t_table	*parser(t_list *token)
{
	int				redir_type;
	t_table			*head;
	t_table			*table;

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
		{
			if (append_redirection(&table, token, redir_type) == -1)
				return (NULL);
		}
		else
			parse_command(&token, &table);
		token = token->next;
	}
	return (head);
}
