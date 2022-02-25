/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/21 20:30:40 by tkruger           #+#    #+#             */
/*   Updated: 2022/02/21 20:32:27 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include "../libs/libft/includes/libft.h"

#include <stdio.h>

extern char	**g_env;

int	main(int argc, char **argv, char **envp)
{
	t_list	*tokens;
	
	if (argc > 1)
		return (0);
	// tokens = split_tok("test");
	tokens = split_tok("\'test hello \'hi|F|SDFSFASF\">><<<><>>\">>>");

	int i = 0;
	while (tokens != NULL)
	{
		printf("%s}\n", tokens->content);
		tokens = tokens->next;
	}
	environment_init(envp);
	return (0);
}
