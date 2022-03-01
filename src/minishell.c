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
#include <errno.h>

extern char	**g_env;
int	cd_builtin(char const **arguments);
int	pwd_builtin(char const **arguments);


int	main(int argc, char **argv, char **envp)
{
	t_list		*tokens;
	char		**arr;

	environment_init(envp);
	arr = malloc(1 * sizeof(char *));
	arr[1] = NULL;
	arr[0] = ft_strdup(NULL);
	printf("%d\n", env_builtin(NULL));
	if (argc > 1)
		return (0);
	tokens = lexer("echo \"this text is redirected to a file!\" > textfile");
	// printf("%s\n", syntax_check(tokens) ? "true" : "false");
	return (0);
}
