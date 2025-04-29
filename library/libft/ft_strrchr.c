/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 17:45:08 by cmontaig          #+#    #+#             */
/*   Updated: 2025/04/11 17:45:15 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	size_t	i;
	char	*lastone;

	lastone = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == (char)c)
			lastone = ((char *)&str[i]);
		i++;
	}
	if (c == '\0')
		return (((char *)&str[i]));
	return (lastone);
}
