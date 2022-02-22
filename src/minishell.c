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
	
	tokens = split_tok("test");
	return (0);
}
