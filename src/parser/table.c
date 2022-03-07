#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/minishell.h"

# define PIPE "|"
# define OUT 1
# define IN 2
# define APPEND 4
# define HEREDOC 8

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

/* an redirection append function handling the creation too 
inside this function we could add the variable expansion and 
the quote removal before creating a redirection object that is 
added to the list */
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
		*type = OUT;
	else if (ft_strcmp((*token)->content, "<") == 0)
		*type = IN;
	else if (ft_strcmp((*token)->content, ">>") == 0)
		*type = APPEND;
	else if (ft_strcmp((*token)->content, "<<") == 0)
		*type = HEREDOC;
	*token = (*token)->next;
	return (*type);
}

/* variable expansion function must be called until there is
no '$' left. */

static int	count(const char *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
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
	expanded_string = (*token)->content;
	if (count(expanded_string, ' ') > 1)
		; // array_append_array;
	else
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

// gcc lexer.c syntax_check.c cmd.c ../libs/libft/libft.a -lreadline
// gcc lexer.c cmd.c ../libs/libft/libft.a -lreadline
// gcc lexer.c ast.c ../libs/libft/libft.a -lreadline

// ../libs/libft/libft.a -I/Users/rjasari/.brew/opt/readline/include -lreadline -L/Users/rjasari/.brew/opt/readline/lib