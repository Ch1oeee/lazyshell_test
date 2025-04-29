/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 14:24:15 by skock             #+#    #+#             */
/*   Updated: 2025/03/18 11:32:03 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

int	ft_strchr(const char *str, int c)
{
	int				i;
	unsigned char	uc;

	i = 0;
	uc = (unsigned char)c;
	if (str == NULL)
		return (0);
	while (str[i])
	{
		if (str[i] == uc)
			return (1);
		i++;
	}
	if (str[i] == uc)
		return (1);
	return (0);
}

char	*ft_strjoin(const char *s1, char const *s2)
{
	char	*str;
	size_t	i;
	size_t	j;

	str = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	if (s1)
	{
		while (s1[i])
			str[j++] = s1[i++];
	}
	i = 0;
	if (s2)
	{
		while (s2[i])
			str[j++] = s2[i++];
	}
	str[j] = '\0';
	return (str);
}

char	*ft_str_n_chr_cpy(const char *s1)
{
	int		j;
	int		i;
	char	*str;

	i = 0;
	j = 0;
	if (!s1)
		return (NULL);
	while (s1[j] != '\n' && s1[j])
		j++;
	if (s1[j] == '\n')
		j++;
	str = malloc((j + 1) * (sizeof(char)));
	if (!str)
		return (NULL);
	i = 0;
	while (i < j)
	{
		str[i] = s1[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_remove_line(char *str, const char *line)
{
	int		i;
	int		j;
	char	*new_str;
	size_t	str_len;
	size_t	line_len;

	if (!str || !line)
		return (NULL);
	str_len = ft_strlen(str);
	line_len = ft_strlen(line);
	if (line_len >= str_len)
	{
		free(str);
		return (NULL);
	}
	new_str = malloc((str_len - line_len + 1) * sizeof(char));
	if (!new_str)
		return (NULL);
	i = line_len;
	j = 0;
	while (str[i])
		new_str[j++] = str[i++];
	new_str[j] = '\0';
	free(str);
	return (new_str);
}
