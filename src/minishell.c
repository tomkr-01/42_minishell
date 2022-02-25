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
#include <stdlib.h>

#include <stdio.h>

char **g_env;

void	environment_init(char **envp)
{
	int	i;

	i = 0;
	g_env = ft_calloc(ft_arrlen(envp), sizeof(*g_env));
	if (g_env == NULL)
		exit(ft_putendl_fd("environment_init(): env ft_calloc error", STDOUT_FILENO));
	while (envp[i] != NULL)
	{
		g_env[i] = ft_strdup(envp[i]);
		i++;
	}
	g_env[i] = NULL;
}
// hello does this sync?????





// this text was added in branch tom



void	environment_print(void)
{
	int	i;

	i = 0;
	printf("arrlen: %zu\n", ft_arrlen(g_env));
	while (g_env[i] != NULL)
	{
		printf("%i: ", i);
		fflush(stdout);
		ft_putendl_fd(g_env[i++], STDOUT_FILENO);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_list	*tokens;
	
	if (argc > 1)
		return (0);
	tokens = split_tok("test");
	environment_init(envp);
	environment_print();
	return (0);
}
