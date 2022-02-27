/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rjasari <rjasari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/21 14:31:30 by rjasari           #+#    #+#             */
/*   Updated: 2021/07/21 17:02:36 by rjasari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include <limits.h>

static void	*ft_calloc(size_t count, size_t size)
{
	size_t			index;
	void			*pointer;
	unsigned char	*holder;

	index = 0;
	holder = malloc(count * size);
	if (holder == NULL)
		return (0);
	while (index < (count * size))
	{
		holder[index] = 0;
		index++;
	}
	pointer = holder;
	return (pointer);
}

static char	*ft_strjoin(char *s1, char const *s2)
{
	char	*joined;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (s1 == NULL || s2 == NULL)
		return (NULL);
	joined = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (joined == NULL)
		return (NULL);
	while (s1[i])
	{
		joined[i] = s1[i];
		i++;
	}
	while (s2[j])
		joined[i++] = s2[j++];
	joined[i] = '\0';
	if (s1)
		free(s1);
	return (joined);
}

static char	*set_ret(int bytes, char **con, char **line)
{
	if (bytes == 0 && !ft_strchr(*con, '\n'))
	{
		*line = ft_substr(*con, 0, ft_strchr(*con, '\0') - *con);
		free(*con);
		*con = NULL;
		if (*line[0] == '\0')
		{
			free(*line);
			return (NULL);
		}
		return (*line);
	}
	*line = ft_substr(*con, 0, (bytes = (ft_strchr(*con, '\n') - *con) + 1));
	*con = ft_memmove(*con, *con + bytes, ft_strlen(*con) + 1 - bytes);
	return (*line);
}

char	*init_content(char *content)
{
	if (!content)
	{
		content = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
		if (content == NULL)
			return (NULL);
	}
	return (content);
}

char	*get_next_line(int fd)
{
	int				bytes;
	char			*buffer;
	char			*line;
	static char		*content[2048];

	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	bytes = read(fd, buffer, BUFFER_SIZE);
	if (fd < 0 || BUFFER_SIZE <= 0 || bytes == -1 || !buffer)
	{
		free(buffer);
		return (NULL);
	}
	buffer[bytes] = '\0';
	content[fd] = init_content(content[fd]);
	while (bytes > 0)
	{
		content[fd] = ft_strjoin(content[fd], buffer);
		if (ft_strchr(content[fd], '\n'))
			break ;
		bytes = read(fd, buffer, BUFFER_SIZE);
		buffer[bytes] = '\0';
	}
	free(buffer);
	return (set_ret(bytes, &content[fd], &line));
}
