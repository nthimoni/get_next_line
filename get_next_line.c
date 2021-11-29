/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nthimoni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 14:23:37 by nthimoni          #+#    #+#             */
/*   Updated: 2021/11/29 22:25:45 by nthimoni         ###   ########.fr       */
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

static int	read_file(int fd, char **prev)
{	
	char	buffer[BUFFER_SIZE + 1];
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
	static char	*prev = NULL;
	char		*ret;

	if (fd == -1 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!prev)
	{
		prev = malloc(1);
		if (!prev)
			return (NULL);
		prev[0] = '\0';
	}
	if (!read_file(fd, &prev))
	{
		free_ptr((void *)&prev);
		return (NULL);
	}
	ret = parse(&prev);
	if (!ret)
		free_ptr((void *)&prev);
	return (ret);
}


#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main()
{
	char	*ptr;
	int fd = open("/mnt/nfs/homes/nthimoni/Desktop/projects42/C/get_next_line/gnlTester/files/big_line_no_nl", O_RDONLY);
	ptr = get_next_line(fd);
	while (ptr)
	{
		printf("%s", ptr);
		free(ptr);
		ptr = get_next_line(fd);
	}
}
