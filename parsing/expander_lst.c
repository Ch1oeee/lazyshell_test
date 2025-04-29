/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_lst.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 15:22:23 by skock             #+#    #+#             */
/*   Updated: 2025/04/05 15:22:45 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*new_expand(char *str)
{
	t_token		*expand;

	expand = malloc(sizeof(t_token));
	expand->value = str;
	expand->next = NULL;
	expand->type = EXPANDING;
	expand->is_next_space = true;
	return (expand);
}

void	expand_add_back(t_token **lst, t_token *new)
{
	t_token	*last;

	if (!*lst)
		*lst = new;
	else
	{
		last = *lst;
		while (last->next != NULL)
			last = last->next;
		last->next = new;
	}
}

void	fill_expand_lst(t_ms *minishell, char *str)
{
	t_token	*expand;

	expand = new_expand(str);
	if (!expand)
		return ;
	expand_add_back(&minishell->expand, expand);
}
