/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkruger <tkruger@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 15:11:04 by tkruger           #+#    #+#             */
/*   Updated: 2022/03/10 18:36:50 by tkruger          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// size_t	prep_strchr(const char *s, int c)
// {
// 	if (ft_strrchr_int(s, c) ==)
// }

char	*quote_remover(char *token)
{
	char	*q_removed;
	char	quote;
	size_t	i;

	i = 0;
	if (ft_strchr(token, '\'') < ft_strchr(token, '"') && ft_strchr(token, '\'') != NULL)
		quote = '\'';
	else
		quote = '\"';
	q_removed = ft_substr(token, 0, ft_strchr_int(token, quote) + 1);
	printf("first 1: %d\n", ft_strchr_int(token, quote));
	printf("second 1: %d\n", ft_strrchr_int(token, quote));
	q_removed = ft_strjoin_free(q_removed,
			ft_substr(token,
				ft_strchr_int(token, quote) + 1, ft_strlen(token) - ft_strrchr_int(token, quote) + 1));
	printf("third: %s\n", q_removed);
	q_removed = ft_strjoin_free(q_removed,
			ft_substr_free(token,
				ft_strrchr_int(token, quote) + 1, ft_strlen(token)));
	return (q_removed);
}

int main(void)
{
	printf("%s|", quote_remover(ft_strdup("this\" is a s\"tring")));
}