/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rjasari <rjasari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/21 14:30:48 by rjasari           #+#    #+#             */
/*   Updated: 2021/08/13 12:28:34 by rjasari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <unistd.h>
# include <stdlib.h>

char	*get_next_line(int fd);

void	*ft_memmove(void *dest, const void *src, size_t len);
size_t	ft_strlen(const char *string);
char	*ft_strdup(const char *s1);
char	*ft_strchr(const char *string, int c);
char	*ft_substr(char const *s, unsigned int start, size_t len);

#endif