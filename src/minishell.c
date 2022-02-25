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

void	environment_init(void)
{
	g_env = malloc(5 * sizeof(*g_env));
	if (g_env == NULL)
		exit(ft_putendl_fd("environment_init: env malloc error", STDOUT_FILENO));
	g_env[0] = ft_strjoin("USER=", getenv("USER"));
	g_env[1] = ft_strjoin("PATH=", getenv("PATH"));
	g_env[2] = ft_strjoin("HOME=", getenv("HOME"));
	g_env[3] = ft_strjoin("PWD=", getenv("PWD"));
	g_env[4] = ft_strjoin("OLDPWD=", getenv("OLDPWD"));
	g_env[5] = NULL;
}

void	environment_export(char *export_var)
{
	int		i;
	char	**new_env;

	i = 0;
	new_env = ft_calloc(ft_arrlen(g_env) + 1, sizeof(*g_env));
	while (g_env[i] != NULL)
	{
		new_env[i] = g_env[i];
		i++;
	}
	new_env[i] = export_var;
	new_env[i + 1] = NULL;
	free(g_env);
	g_env = new_env;
	new_env = NULL;
}

void	environment_unset(char *unset_var)
{
	int		i;
	int		j;
	char	**new_env;

	i = 0;
	j = 0;
	new_env = ft_calloc(ft_arrlen(g_env) - 1, sizeof(*g_env));
	new_env[ft_arrlen(g_env)] = NULL;
	while (g_env[i] != NULL)
	{
		if (ft_strncmp(g_env[i], unset_var, ft_strlen(unset_var)) != 0)
			new_env[j] = g_env[i];
		else
			free(g_env[i++]);
		i++;
		j++;
	}
	
}

void	environment_print(void)
{
	int	i;

	i = 0;
	printf("arrlen: %i\n", ft_arrlen(g_env));
	while (g_env[i] != NULL)
	{
		printf("%i: ", i);
		fflush(stdout);
		ft_putendl_fd(g_env[i++], STDOUT_FILENO);
	}
}

int	main(void)
{
	t_list	*tokens;
	
	tokens = split_tok("test");
	environment_init();
	environment_print();
	environment_export(ft_strdup("TEST=doesitworktho??"));
	environment_print();
	return (0);
}
