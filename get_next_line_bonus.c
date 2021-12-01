/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nthimoni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 14:23:37 by nthimoni          #+#    #+#             */
/*   Updated: 2021/12/01 15:51:13 by nthimoni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <unistd.h>

static void	free_ptr(void **ptr)
{
	free(*ptr);
	*ptr = NULL;
}

static char	*parse(char **prev)
{
	char	*ret;
	char	*tmp;
	char	*endl;

	if (!*prev)
		return (NULL);
	if (!ft_strchr(*prev, '\n'))
	{
		ret = ft_substr(*prev, 0, ft_strlen(*prev));
		if (!ret)
			return (NULL);
		free_ptr((void *)prev);
		return (ret);
	}
	endl = ft_strchr(*prev, '\n');
	tmp = *prev;
	ret = ft_substr(*prev, 0, endl - *prev + 1);
	*prev = ft_substr(*prev, endl - *prev + 1, ft_strlen(endl + 1));
	free(tmp);
	if (!*prev)
		free_ptr((void *)&ret);
	return (ret);
}

static int	read_file(int fd, char **prev, char *buffer)
{	
	int		bytes;
	char	*tmp;

	bytes = 1;
	while (!ft_strchr(*prev, '\n') && bytes != 0)
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes == -1)
			return (0);
		if (bytes == 0 && ft_strlen(*prev) == 0)
			return (0);
		buffer[bytes] = '\0';
		tmp = *prev;
		*prev = ft_strjoin(*prev, buffer);
		free(tmp);
		if (!*prev)
			return (0);
	}
	return (1);
}

char	*get_next_line(int fd)
{
	static char	*prev[OPEN_MAX];
	char		*ret;
	char		*buffer;

	if (fd == -1 || BUFFER_SIZE <= 0 || read(fd, prev, 0))
		return (NULL);
	if (!prev[fd])
	{
		prev[fd] = malloc(1);
		if (!prev[fd])
			return (NULL);
		prev[fd][0] = '\0';
	}
	buffer = malloc(BUFFER_SIZE + 1);
	if (!read_file(fd, &prev[fd], buffer))
	{
		free_ptr((void *)&prev[fd]);
		free(buffer);
		return (NULL);
	}
	free(buffer);
	ret = parse(&prev[fd]);
	if (!ret)
		free_ptr((void *)&prev[fd]);
	return (ret);
}
