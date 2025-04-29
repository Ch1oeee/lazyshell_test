/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 14:24:12 by skock             #+#    #+#             */
/*   Updated: 2025/01/04 16:17:46 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	start_function(int fd, char **temp)
{
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (0);
	*temp = malloc((BUFFER_SIZE + 1) * (sizeof(char)));
	if (!*temp)
		return (0);
	return (1);
}

char	*get_next_line(int fd)
{
	static char		*str = NULL;
	char			*temp;
	char			*line;
	int				bytes_read;

	if (start_function(fd, &temp) == 0)
		return (NULL);
	while (!ft_strchr(str, '\n'))
	{
		bytes_read = read(fd, temp, BUFFER_SIZE);
		if (bytes_read <= 0)
		{
			free(temp);
			if (bytes_read == 0)
				break ;
			return (NULL);
		}
		temp[bytes_read] = '\0';
		str = ft_strjoin(str, temp);
	}
	if (ft_strchr(str, '\n'))
		free(temp);
	line = ft_str_n_chr_cpy(str);
	str = ft_remove_line(str, line);
	return (line);
}
