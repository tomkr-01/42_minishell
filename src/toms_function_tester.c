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

extern t_minishell	g_msh;

int	builtins(char **arguments);
int	env_builtin(char **arguments);

char	*pwd_helper();

int	main(int argc, char **argv, char **envp)
{
	t_list		*tokens;
	char		**arr;

	arr = malloc(5 * sizeof(*arr));
	arr[0] = ft_strdup("export");
	arr[1] = ft_strdup("newvar=test");
	arr[2] = ft_strdup("anothervar=anothertext");
	arr[3] = ft_strdup("thirdone=hello");
	arr[4] = NULL;
	environment_init(envp);
	// builtins(&arr[0]);
	// env_builtin(&arr[4]);
	builtins(&argv[1]);
	// env_builtin(&arr[4]);
	printf("exit_value: %d\n", g_msh.exit_code);
	free_array(&arr);
	system("leaks minishell | grep \"total leaked bytes\"");
	// tokens = lexer("echo \"this text is redirected to a file!\" > textfile");
	// printf("%s\n", syntax_check(tokens) ? "true" : "false");
	return (EXIT_SUCCESS);
}
