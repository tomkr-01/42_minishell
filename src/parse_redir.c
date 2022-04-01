/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/01 14:26:16 by tkruger           #+#    #+#             */
/*   Updated: 2022/04/01 14:26:17 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static t_redirection	*create_redirection(int type, char *name)
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

int	append_redirection(t_redirection **redir, t_list *token, int redir_type)
{
	t_redirection	*new;
	t_redirection	*tmp;

	tmp = *redir;
	if (redir_type == HEREDOC)
		heredoc(&token->content);
	if (redir_type == HEREDOC && token->content == NULL)
		return (-1);
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
