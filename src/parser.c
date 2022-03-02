/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/28 22:01:02 by tkruger           #+#    #+#             */
/*   Updated: 2022/03/02 18:15:01 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// this function goes through one command with arguments and redirections until
// a pipe and creates a command struct from the tokens
char	**parser(t_list *tokens)
{
	t_list	*token_parser;
	t_cmd	*new_cmd;
	size_t	i;

	token_parser = tokens;
	i = 0;
	while (ft_strncmp(token_parser->content, "|", 1) != 0)
	{
		token_parser = token_parser->next;
		i++;
	}
	new_cmd = ft_calloc(i, sizeof(t_cmd));
	// we would need the possibility to free all other memory in case we don't want to rely on exit()
	if (new_cmd == NULL)
		exit(ft_putendl_fd("parser(): cmd_table malloc fail", STDERR_FILENO));
	token_parser = tokens;
	// this loop sets the t_cmd struct (see the header file!) with the according variables for one simple command. For multiple commands seperated by pipes, we will need to run this function in a loop
	// First I check for the pipe because it ends a simple command
	// Then I check for redirections because the word after a redir operator is the filename or delimiter which I store in an array so that there can be multiple
	// Then I check for the rest and if I haven't parsed an executable name, I save it
	// Lastly, I add everything else to the arguments array
	while (token_parser != NULL)
	{
		if (ft_strncmp(token_parser, "|", 2) == 0)
		{
			new_cmd->pipe = true;
			break ;
		}
		else if (ft_strncmp(token_parser, ">", 2) == 0)
		{
			new_cmd->out_redirs = add_array_element(new_cmd->out_redirs, token_parser->next->content);
			token_parser = token_parser->next; // I need to skip one additional token because I don't save the actual ">" in the array. If the array element != NULL, we know that there is a redirection
		}
		else if (ft_strncmp(token_parser, ">>", 3) == 0)
		{
			new_cmd->appends = add_array_element(new_cmd->appends, token_parser->next->content);
			token_parser = token_parser->next;
		}
		else if (ft_strncmp(token_parser, "<", 2) == 0)
		{
			new_cmd->in_redir = ft_strdup(token_parser->next->content); // there can be only one input-redirection, right? otherwise well have to change the struct as well as this code
			token_parser = token_parser->next;
		}
		else if (ft_strncmp(token_parser, "<<", 2) == 0)
		{
			new_cmd->delimiter = ft_strdup(token_parser->next->content); // there can be only one delimiter, right? otherwise well have to change the struct as well as this code
			token_parser = token_parser->next;
		}
		else if (new_cmd->executable == NULL)
		{
			new_cmd->executable = ft_strdup(token_parser->content);
		}
		else
		{
			new_cmd->arguments = add_array_element(new_cmd->arguments, token_parser->content);
		}
	}
	new_cmd->next = NULL; // this pointer will point to the next command if there is a pipe or NULL
}
