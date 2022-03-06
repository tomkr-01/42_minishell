#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/minishell.h"

# define PIPE "|"

typedef struct s_redirection {
	int						type;
	char					*name;
	struct s_redirection	*next;
}			t_redirection;

typedef struct s_table {
	char					**arguments;
	t_redirection			*redirections;
	struct s_table			*next;
}			t_table;

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

void	append_redirection(t_table **lst, t_redirection *new)
{
	t_table	*temporary;

	temporary = *lst;
	if (temporary->redirections == NULL)
	{
		temporary->redirections = new;
		return ;
	}
	if (new == NULL)
		return ;
	while (temporary->redirections->next != NULL)
		temporary->redirections = temporary->redirections->next;
	new->next = temporary->redirections->next;
	temporary->redirections->next = new;
}

/* an redirection append function handling the creation too */
// void	append_redirection(t_table **lst, t_list *token, int redir_type)
// {
// 	t_redirection	*new_redirection;
// 	t_table			*temporary;

// 	new_redirection = create_redirection(redir_type, token->content);
// 	temporary = *lst;
// 	if (temporary->redirections == NULL)
// 	{
// 		temporary->redirections = new_redirection;
// 		return ;
// 	}
// 	if (new_redirection == NULL)
// 		return ;
// 	while (temporary->redirections->next != NULL)
// 		temporary->redirections = temporary->redirections->next;
// 	new_redirection->next = temporary->redirections->next;
// 	temporary->redirections->next = new_redirection;
// }

int	find_redirection_type(t_list **token, int *type)
{
	if (ft_strcmp((*token)->content, ">") == 0)
		*type = 1;
	else if (ft_strcmp((*token)->content, "<") == 0)
		*type = 2;
	else if (ft_strcmp((*token)->content, ">>") == 0)
		*type = 4;
	else if (ft_strcmp((*token)->content, "<<") == 0)
		*type = 8;
	*token = (*token)->next;
	return (*type);
}

void	parse_command(t_list **token, t_table **table)
{
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
		if (find_redirection_type(&token, &redir_type) > 0)
		{
			new_redirection = create_redirection(redir_type, token->content);
			append_redirection(&table, new_redirection);
		}
		else
			parse_command(&token, &table);
		token = token->next;
	}
	return (head);
}
