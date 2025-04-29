/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_remove.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 12:32:42 by skock             #+#    #+#             */
/*   Updated: 2025/04/01 11:59:04 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*quote_rmv(const char *str)
{
	char	*new_str;

	if (str[0] == '\"' && str[1] == '\"')
	{
		new_str = ft_calloc(sizeof(char), 1);
		return (new_str);
	}
	if (str[0] == '\'' && str[1] == '\'')
	{
		new_str = ft_calloc(sizeof(char), 1);
		return (new_str);
	}
	new_str = ft_substr(str, 1, ft_strlen(str) - 2);
	return (new_str);
}

void	clear_quote(t_ms *minishell)
{
	t_token	*tmp;
	t_token	*next;

	tmp = minishell->token;
	while (tmp)
	{
		next = tmp->next;
		if (tmp->type == D_QUOTE || tmp->type == S_QUOTE)
		{
			tmp->value = quote_rmv(tmp->value);
			tmp->type = WORD;
		}
		tmp = next;
	}
}
