#include "../inc/minishell.h"

extern t_minishell	g_msh;

t_table	*create_table_row(void)
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
	char	*lim;

	signal(SIGINT, &heredoc_signals);
	lim = ft_strjoin(*limiter, "\n");
	here_string = ft_strdup("");
	while (1)
	{
		line = minishell_get_next_line(STDIN_FILENO);
		if (line == NULL || ft_strcmp(line, lim) == 0)
		{
			ft_free((void **)&line);
			break ;
		}
		here_string = ft_strjoin_free(here_string, line);
		if (here_string == NULL)
			break ;
	}
	ft_free((void **)&lim);
	return (here_string);
}

char	*heredoc_readline(char **limiter)
{
	char	*line;
	char	*here_string;

	signal(SIGINT, &heredoc_signals);
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
		here_string = ft_strjoin_free(here_string, line);
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
	limiter = quote_remover(ft_strdup(*token_content));
	if (ft_strcmp(*token_content, limiter) == 0)
		expansion = true;
	ft_free((void **)token_content);
	if (isatty(STDIN_FILENO))
		*token_content = heredoc_readline(&limiter);
	else
		*token_content = heredoc_get_next_line(&limiter);
	ft_free((void **)&limiter);
	if (close(STDIN_FILENO) == -1)
		ft_free((void **)token_content);
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
	new = create_redirection(redir_type, ft_strdup(token->content));
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
	int		status;
	char	*expanded_string;
	char	**argument_list;

	status = 0;
	expanded_string = expander(ft_strdup((*token)->content), false);
	if (ft_strchr(expanded_string, '\"') || ft_strchr(expanded_string, '\''))
		status = 1;
	expanded_string = quote_remover(expanded_string);
	if (status == 0)
	{
		argument_list = ft_split(expanded_string, ' ');
		(*table)->arguments = array_append_array((*table)->arguments,
				argument_list);
	}
	else
		(*table)->arguments = add_array_element((*table)->arguments,
				expanded_string);
	ft_free((void **)&expanded_string);
}

t_table	*parser(t_list *token)
{
	int				type;
	t_table			*head;
	t_table			*table;

	table = create_table_row();
	if (table == NULL)
		return (NULL);
	head = table;
	while (token != NULL)
	{
		type = 0;
		if (ft_strcmp(token->content, PIPE) == 0)
			table->next = create_table_row();
		if (ft_strcmp(token->content, PIPE) == 0)
			table = table->next;
		else if (find_redirection_type(&token, &type) > 0)
		{
			if (append_redirection(&table->redirections, token, type) == -1)
				return (NULL);
		}
		else
			parse_command(&token, &table);
		token = token->next;
	}
	return (head);
}
