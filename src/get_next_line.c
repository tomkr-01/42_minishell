/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rjasari <rjasari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 11:44:57 by rjasari           #+#    #+#             */
/*   Updated: 2022/03/02 10:29:55 by rjasari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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

static char	*init_content(char *content)
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
	static char		*content;

	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	bytes = read(fd, buffer, BUFFER_SIZE);
	if (fd < 0 || BUFFER_SIZE <= 0 || bytes < 0 || !buffer)
	{
		free(buffer);
		return (NULL);
	}
	buffer[bytes] = '\0';
	content = init_content(content);
	while (bytes > 0)
	{
		content = ft_strjoin_free(content, buffer);
		if (ft_strchr(content, '\n'))
			break ;
		bytes = read(fd, buffer, BUFFER_SIZE);
		buffer[bytes] = '\0';
	}
	free(buffer);
	return (set_ret(bytes, &content, &line));
}
