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
int	check_builtins(char const **arguments);


int	main(int argc, char **argv, char **envp)
{
	t_list		*tokens;
	char const	**arr;

	environment_init(envp);
	arr = malloc(5 * sizeof(char *));
	arr[0] = ft_strdup("exit");
	arr[1] = ft_strdup("NULL");
	arr[2] = ft_strdup("cd");
	arr[3] = ft_strdup("../../Files/Code");
	arr[4] = NULL;
	printf("%d\n", check_builtins(arr));
	// system("leaks minishell");
	if (argc > 1)
		return (0);
	// tokens = lexer("echo \"this text is redirected to a file!\" > textfile");
	// printf("%s\n", syntax_check(tokens) ? "true" : "false");
	return (0);
}
