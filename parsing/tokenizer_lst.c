/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_lst.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:20:19 by skock             #+#    #+#             */
/*   Updated: 2025/04/05 14:42:27 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*new_token(char *str, t_ms *minishell, t_type type)
{
	t_token		*token;
	static int	i = 1;

	token = malloc(sizeof(t_token));
	token->value = str;
	token->next = NULL;
	token->type = type;
	token->index = i++;
	token->is_next_space = false;
	if (minishell->is_next_space == true)
	{
		token->is_next_space = true;
		minishell->is_next_space = false;
	}
	return (token);
}

void	token_add_back(t_token **lst, t_token *new)
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

void	fill_token_list(t_ms *minishell, char *str, t_type type)
{
	t_token	*token;

	token = new_token(str, minishell, type);
	if (!token)
		return ;
	token_add_back(&minishell->token, token);
}
