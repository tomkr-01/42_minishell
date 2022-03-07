#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include "../inc/minishell.h"
#include <stdlib.h>
#include <string.h>

# define PIPE "|"
# define PIPELINE 1
# define COMMAND 2

char	**add_array_element(char **old_arr, char *new_el)
{
	size_t	i;
	size_t	arr_size;
	char	**new_arr;

	i = 0;
	arr_size = ft_arrlen(old_arr);
	new_arr = (char **)malloc((arr_size + 2) * sizeof(char *));
	if (new_arr == NULL)
	{
		perror(new_el);
		exit(0);
	}
	while (i < arr_size)
	{
		new_arr[i] = ft_strdup(old_arr[i]);
		// free(old_arr[i]);
		// old_arr[i] = NULL;
		i++;
	}
	if (old_arr != NULL)
		free(old_arr);
	old_arr = NULL;
	new_arr[i] = ft_strdup(new_el);
	new_arr[i + 1] = NULL;
	return (new_arr);
}

/* struct for an ast structure */

typedef struct s_redir {
	int					mode;
	char				*name;
	struct s_redir		*next;
}			t_redir;

typedef struct s_node {
	int					mode;
	bool				pipe;
	char				**arguments;
	t_redir				*redirections;
	struct s_node		*left;
	struct s_node		*right;
}			t_node;

t_redir	*create_redirection(int type, char *name);

t_node	*create_node(void)
{
	t_node		*node;

	node = (t_node *)malloc(sizeof(t_node));
	if (node == NULL)
		return (NULL);
	// think about setting the mode to COMMAND by default
	// and only change it when we find a pipeline
	node->mode = COMMAND;
	node->pipe = false;
	node->arguments = NULL;
	// if this below is NULL the redirections are not appended to the list
	node->redirections = create_redirection(0, NULL);
	node->left = NULL;
	node->left = NULL;
}

t_redir	*create_redirection(int type, char *name)
{
	t_redir		*new_redirection;

	new_redirection = (t_redir *)malloc(sizeof(t_redir));
	if (new_redirection == NULL)
		return (NULL);
	new_redirection->mode = type;
	new_redirection->name = name;
	new_redirection->next = NULL;
	return (new_redirection);
}

t_redir	*parse_redirection(t_list **token)
{
	int			type;
	t_redir		*new_redirection;

	type = 0;
	if (ft_strcmp((*token)->content, "<") == 0)
		type = 1;
	else if (ft_strcmp((*token)->content, ">") == 0)
		type = 2;
	else if (ft_strcmp((*token)->content, "<<") == 0)
		type = 4;
	else if (ft_strcmp((*token)->content, ">>") == 0)
		type = 8;
	*token = (*token)->next;
	new_redirection = create_redirection(type, (*token)->content);
	return (new_redirection);
}

void	append_redirection(t_redir **list, t_redir *new)
{
	t_redir		*temp;

	temp = *list;
	if (temp == NULL)
	{
		temp = new;
		return ;
	}
	if (new == NULL)
		return ;
	while (temp->next != NULL)
		temp = temp->next;
	new->next = temp->next;
	temp->next = new;
}

int	count_pipes(t_list *token)
{
	int		count;

	count = 0;
	while (token != NULL)
	{
		if (ft_strcmp(token->content, PIPE) == 0)
			count++;
		token = token->next;
	}
	return (count);
}

/* example thought:
		the first command is parsed into the left node
		-> parsing function must return a filled node
		and the starting node becomes the right node.
		if we have a pipeline again we repeat the same pattern
		if we don't we return a filled node to the current node
		(in an else statement). this catches the case for a pipe
		between to commands
		*/

t_node	*parser(t_list *token)
{
	int			pipes;
	t_node		*root;
	t_node		*node;
	t_redir		*redirection;

	pipes = count_pipes(token);
	node = create_node();
	root = node;
	/* just a command must be executed differently as we don't pipe
	and therefore we don't have to worry about pipes at all */
	while (token != NULL)
	{
		if (pipes > 0)
		{
			node->mode = PIPELINE;
			node->pipe = true;
			node->left = create_node();
			node->right = create_node();
			pipes--;
		}
		if (node->mode == PIPELINE)
		{
			while (token != NULL && ft_strcmp(token->content, PIPE) != 0)
			{
				if (!ft_strcmp(token->content, "<") 
					|| !ft_strcmp(token->content, ">"))
				{
					redirection = parse_redirection(&token);
					append_redirection(&(node->left->redirections), redirection);
				}
				else
					node->left->arguments = add_array_element(node->left->arguments,
																token->content);
				token = token->next;
			}
			node = node->right;
			token = token->next;
		}
		else
		{
			while (token != NULL && ft_strcmp(token->content, PIPE) != 0)
			{
				if (!ft_strcmp(token->content, "<") 
					|| !ft_strcmp(token->content, ">"))
				{
					redirection = parse_redirection(&token);
					append_redirection(&node->redirections, redirection);
				}
				else
					node->arguments = add_array_element(node->arguments,
																token->content);
				token = token->next;
			}
		}
	}
	return (root);
}

int	main(int argc, char *argv[], char **envp)
{
	t_list	*tokens;
	t_node	*ast;

	tokens = lexer("ls < cmd.c > out -l | wc -l | grep 15");
	ast = parser(tokens);
	return (0);
}
