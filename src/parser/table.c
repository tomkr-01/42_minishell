#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "../../inc/minishell.h"

t_minishell	g_msh;

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

/* an redirection append function handling the creation too 
inside this function we could add the variable expansion and 
the quote removal before creating a redirection object that is 
added to the list */

/* if a filename is extracted and the string contains a whitespace
we return: minishell: $woco: ambigious redirect */
void	append_redirection(t_table **lst, t_list *token, int redir_type)
{
	// create variable to expand variable into, if error message
	// the token without expansion must be given back
	t_table			*temporary;
	t_redirection	*new_redirection;

	temporary = *lst;
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

/* variable expansion function must be called until there is
no '$' left. */

static int	count(const char *s, char c)
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



///////////////////////////////////////////////

int	main(int argc, char *argv[], char **envp)
{
	char	*exe;
	char	*command[2];
	t_list	*tokens;
	t_table	*table;
	environment_init(envp);
	tokens = (t_list *)lexer("cat < cmd.c");
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