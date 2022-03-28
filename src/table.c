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
	here_string = NULL;
	*limiter = ft_strjoin_free(*limiter, ft_strdup("\n"));
	here_string = ft_strdup("");
	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (line == NULL || ft_strcmp(line, *limiter) == 0)
		{
			ft_free((void **)&line);
			break ;
		}
		here_string = ft_strjoin_free(here_string, line);
		if (here_string == NULL)
			break ;
	}
	return (here_string);
}

char	*heredoc_readline(char **limiter)
{
	char	*line;
	char	*here_string;

	signal(SIGINT, &heredoc_signals);
	here_string = NULL;
	here_string = ft_strdup("");
	while (1)
	{
		line = readline("> ");
		if (line == NULL || ft_strcmp(line, *limiter) == 0)
		{
			ft_free((void **)&line);
			break ;
		}
		line = ft_strjoin_free(line, ft_strdup("\n"));
		here_string = ft_strjoin_free(here_string, line);;
		if (here_string == NULL)
			break ;
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
	limiter = quote_remover(ft_strdup(*token_content));
	if (ft_strcmp(delimiter_copy, limiter) == 0)
		expansion = true;
	ft_free((void **)&delimiter_copy);
	if (isatty(STDIN_FILENO))
		*token_content = heredoc_readline(&limiter);
	else
		*token_content = heredoc_get_next_line(&limiter);
	ft_free((void **)&limiter);
	if (close(STDIN_FILENO) == -1)
		ft_free((void**)token_content);
	if (dup2(fd, STDIN_FILENO) == -1)
		close(fd);
	close(fd);
	if (expansion)
		*token_content = expander(*token_content, false);
}

int	append_redirection(t_redirection **redir, t_list *token, int redir_type)
{
	t_redirection	*new;
	t_redirection	*tmp;

	tmp = *redir;
	if (redir_type == HEREDOC)
	{
		heredoc(&token->content);
		if (token->content == NULL)
			return (-1);
	}
	new = create_redirection(redir_type, token->content);
	if (*redir == NULL)
	{
		*redir = new;
		return (0);
	}
	if (new == NULL)
		return (-1);
	while (tmp->next != NULL)
		tmp = tmp->next;
	new->next = tmp->next;
	tmp->next = new;
	return (0);
}

// int	append_redirection(t_table **lst, t_list *token, int redir_type)
// {
// 	t_table			*tmp;
// 	t_redirection	*new_redirection;
// 	t_redirection	*tmp_redir;

// 	// tmp = *lst;
// 	tmp_redir = tmp->redirections;
// 	// printf("%d. %s\n", redir_type, token->content);
// 	if (redir_type == HEREDOC)
// 	{
// 		heredoc(&token->content);
// 		if (token->content == NULL)
// 			return (-1);
// 	}
// 	new_redirection = create_redirection(redir_type, token->content);
// 	// printf("%p\n", new_redirection);
// 	if (tmp->redirections == NULL)
// 	{
// 		tmp->redirections = new_redirection;
// 		return (0);
// 	}
// 	if (new_redirection == NULL)
// 		return (-1);
// 	while (tmp->redirections->next != NULL)
// 		tmp->redirections = tmp->redirections->next;
// 	new_redirection->next = tmp->redirections->next;
// 	tmp->redirections->next = new_redirection;
// 	(*lst)->redirections = tmp_redir;
// 	return (0);
// }

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
	expanded_string = expander(ft_strdup((*token)->content), true);
	if (((*table)->arguments != NULL && check_builtins((*table)->arguments))
			|| count(expanded_string, ' ') == 1)
		(*table)->arguments = add_array_element((*table)->arguments, expanded_string);
	else if (count(expanded_string, ' ') > 1)
	{
		argument_list = ft_split(expanded_string, ' ');
		(*table)->arguments = array_append_array((*table)->arguments, argument_list);
		// ft_free_array(&argument_list);
	}
	free(expanded_string);
	return ;
}

int	print_table(t_table **head)
{
	t_table		*start;

	printf("printing after parsing:\n");
	start = *head;
	while (start != NULL)
	{
		while (start->redirections != NULL)
		{
			printf("%s\n", start->redirections->name);
			start->redirections = start->redirections->next;
		}
		start = start->next;
	}
	return (0);
}

void	free_command_table(t_table *table)
{
	t_table			*head;
	t_table			*next;
	t_redirection	*redir_head;
	t_redirection	*redir_next;

	head = table;
	while (table != NULL)
	{
		redir_head = table->redirections;
		while (table->redirections != NULL)
		{
			free(table->redirections->name);
			table->redirections->name = NULL;
			table->redirections = table->redirections->next;
		}
		free(redir_head);
		ft_free_array(&table->arguments);
		table = table->next;
	}
	free(head);
	head = NULL;
}

t_table	*parser(t_list *token)
{
	int				type;
	t_table			*head;
	t_table			*table;
	t_table			*copy;

	table = create_table_row();
	if (table == NULL)
		return (NULL);
	head = table;
	copy = table;
	while (token != NULL)
	{
		type = 0;
		if (ft_strcmp(token->content, PIPE) == 0)
		{
			table->next = create_table_row();
			table = table->next;
		}
		else if (find_redirection_type(&token, &type) > 0)
		{
			if (append_redirection(&table->redirections, token, type) == -1)
				return (NULL);
		}
		else
			parse_command(&token, &table);
		token = token->next;
	}
	ft_lstclear(&token, &del_content);
	return (head);
}
