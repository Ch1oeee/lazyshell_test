/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:13:04 by skock             #+#    #+#             */
/*   Updated: 2024/11/12 16:39:42 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**ft_free_all(char **new_str, int j)
{
	while (j > 0)
		free(new_str[--j]);
	free(new_str);
	return (NULL);
}

static int	ft_count_word(const char *s, char c)
{
	int	i;
	int	count;
	int	in_word;

	i = 0;
	count = 0;
	in_word = 0;
	while (s[i])
	{
		if (s[i] != c && in_word == 0)
		{
			in_word = 1;
			count++;
		}
		else if (s[i] == c)
			in_word = 0;
		i++;
	}
	return (count);
}

static char	**ft_malloc_word(const char *str, char **new_str, char c)
{
	int	i;
	int	j;
	int	token;

	i = 0;
	j = 0;
	while (str[i])
	{
		token = 0;
		while (str[i] == c)
			i++;
		while (str[i] != c && str[i] != '\0')
		{
			token++;
			i++;
		}
		if (token > 0)
		{
			new_str[j] = ft_calloc((token + 1), sizeof(char));
			if (!new_str[j])
				return (ft_free_all(new_str, j));
			j++;
		}
	}
	return (new_str);
}

static char	**ft_put(const char *str, char **new_str, char c)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	while (str[i])
	{
		while (str[i] == c)
			i++;
		if (str[i] != '\0')
		{
			k = 0;
			while (str[i] != c && str[i] != '\0')
			{
				new_str[j][k++] = str[i++];
			}
			new_str[j][k] = '\0';
			j++;
		}
	}
	new_str[j] = NULL;
	return (new_str);
}

char	**ft_split(char const *s, char c)
{
	char	**new_str;
	int		word_nb;

	if (!s)
		return (NULL);
	word_nb = ft_count_word(s, c);
	new_str = malloc((word_nb + 1) * sizeof(char *));
	if (!new_str)
		return (NULL);
	if (!ft_malloc_word(s, new_str, c))
		return (NULL);
	new_str = ft_put(s, new_str, c);
	return (new_str);
}
