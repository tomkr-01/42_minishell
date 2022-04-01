#include "../inc/minishell.h"

extern t_minishell	g_msh;

static t_table	*create_table_row(void)
{
	t_table	*row;

	row = (t_table *)malloc(sizeof(t_table));
	if (row == NULL)
		return (NULL);
	row->arguments = NULL;
	row->redirections = NULL;
	row->next = NULL;
	return (row);
}

static int	find_redirection_type(t_list **token, int *type)
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

static void	parse_command(t_list **token, t_table **table)
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
	int		type;
	t_table	*head;
	t_table	*table;

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
