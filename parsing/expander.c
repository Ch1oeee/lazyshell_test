/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 12:02:46 by skock             #+#    #+#             */
/*   Updated: 2025/04/05 15:26:13 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	join_expand(t_ms *minishell, int index)
{
	int		size;
	char	*word_expand;
	t_token	*tmp;

	size = expand_size(minishell);
	tmp = minishell->expand;
	word_expand = tmp->value;
	while (tmp->next && tmp && size >= 0)
	{
		word_expand = ft_strjoin(word_expand, tmp->next->value);
		tmp = tmp->next;
		size--;
	}
	modify_main_token_lst(minishell, word_expand, index);
	free_expand_list(minishell);
}

void	expand(t_ms *minishell)
{
	t_env	*tmp_env;
	t_token	*tmp2;

	tmp2 = minishell->expand;
	while (tmp2)
	{
		tmp_env = minishell->env_lst;
		while (tmp_env)
		{
			if (!ft_strcmp(tmp_env->key, tmp2->value + 1))
			{
				free(tmp2->value);
				tmp2->value = ft_strdup(tmp_env->value);
			}
			else if (tmp2->value[0] == '$'
				&& ft_strcmp(tmp_env->key, tmp2->value + 1) == 1)
			{
				free(tmp2->value);
				tmp2->value = ft_strdup("");
			}
			tmp_env = tmp_env->next;
		}
		tmp2 = tmp2->next;
	}
	return ;
}

void	do_expand(char *value, t_ms *minishell, int index)
{
	int	i;

	i = 0;
	while (value[i])
	{
		if (value[i] == '$')
		{
			dollar_expand(value, minishell, &i);
			if (!value[i])
				break ;
		}
		else if (value[i] != '$')
		{
			word_expand(value, minishell, &i);
			if (!value[i])
				break ;
		}
		i++;
	}
	expand(minishell);
	join_expand(minishell, index);
}

void	expand_token(t_token *token, t_ms *minishell)
{
	t_token	*tmp;

	minishell->expand = NULL;
	tmp = token;
	while (tmp)
	{
		if (tmp->type == S_QUOTE)
			tmp = tmp->next;
		else if (tmp->type == WORD)
		{
			do_expand(tmp->value, minishell, tmp->index);
			tmp = tmp->next;
		}
		else if (tmp->type == D_QUOTE)
		{
			do_expand(tmp->value, minishell, tmp->index);
			tmp = tmp->next;
		}
		else
			tmp = tmp->next;
	}
}
