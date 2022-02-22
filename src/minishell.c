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

int	main(int argc, char **argv)
{
	t_list	*tokens;
	char	*line = "\'\"hi||><<\"\'|  <<\"hel|<>>lo\"\"another one\"'noice!!'test  ||text3||>><<<<>><>< 'text1'";

	printf("%s\n", line);

	tokens = split_tok(line);
	printf("tokens: %i\n", ft_lstsize(tokens));
	while (tokens != NULL)
	{
		printf("%s}\n", tokens->content);
		tokens = tokens->next;
	}
	

	return (0);
}

// https://brennan.io/2015/01/16/write-a-shell-in-c/