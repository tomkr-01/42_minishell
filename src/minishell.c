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
#include <string.h>

extern char	**g_env;
int	builtins(char **arguments);

int	main(int argc, char **argv, char **envp)
{
	t_list		*tokens;

	environment_init(envp);
	tokens = lexer("export var=test\"st|ring\"end not part of var anymore");
	while (tokens != NULL)
	{
		printf("%s\n", tokens->content);
		tokens = tokens->next;
	}
	builtins(&argv[1]);
	printf("%s\n", get_var("?"));
	// tokens = lexer("echo \"this text is redirected to a file!\" > textfile");
	// printf("%s\n", syntax_check(tokens) ? "true" : "false");
	return (EXIT_SUCCESS);
}
