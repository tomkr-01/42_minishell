/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rjasari <rjasari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/21 14:31:14 by rjasari           #+#    #+#             */
/*   Updated: 2021/08/13 12:29:00 by rjasari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

size_t 	ft_strlen(const char *string)
{
	size_t	string_length;

	string_length = 0;
	while (string[string_length] != '\0')
	{
		string_length++;
	}
	return (string_length);
}

void	*ft_memmove(void *dest, const void *src, size_t len)
{
	unsigned char		*destination;
	unsigned char		*source;
	size_t				index;

	destination = (unsigned char *)dest;
	source = (unsigned char *)src;
	index = 0;
	if (dest == NULL && src == NULL)
		return (NULL);
	if (source < destination)
	{
		while (len > 0)
		{
			destination[len - 1] = source[len - 1];
			len--;
		}
		return (dest);
	}
	while (index < len)
	{
		destination[index] = source[index];
		index++;
	}
	return (dest);
}

char	*ft_strdup(const char *s1)
{
	size_t	length;
	size_t	index;
	char	*copy;

	length = 0;
	index = 0;
	while (s1[length])
	{
		length++;
	}
	copy = (char *)malloc(sizeof(char) * (length + 1));
	if (copy == NULL)
		return (NULL);
	while (index < length)
	{
		copy[index] = s1[index];
		index++;
	}
	copy[index] = '\0';
	return (copy);
}

char	*ft_strchr(const char *string, int c)
{
	int	index;

	index = 0;
	while (string[index])
	{
		if (string[index] == c)
		{
			return ((char *)(string + index));
		}
		index++;
	}
	if (string[index] == c)
	{
		return ((char *)(string + index));
	}
	else
	{
		return (0);
	}
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substring;
	size_t	index;
	size_t	s_len;

	index = 0;
	s_len = ft_strlen(s);
	if (start >= s_len)
	{
		substring = ft_strdup("");
		return (substring);
	}
	substring = malloc(sizeof(char) * (len + 1));
	if (substring == NULL)
		return (0);
	while (s[index] && index < len)
	{
		substring[index] = s[index + start];
		index++;
	}
	substring[index] = '\0';
	return (substring);
}
