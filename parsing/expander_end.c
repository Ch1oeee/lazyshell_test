/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_end.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 15:21:35 by skock             #+#    #+#             */
/*   Updated: 2025/04/05 15:26:49 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	word_expand(char *value, t_ms *minishell, int *i)
{
	char	*word;
	int		start;

	start = *i;
	while (value[*i] && value[*i] != '$')
		(*i)++;
	word = ft_substr(value, start, (size_t)((*i) - start));
	fill_expand_lst(minishell, word);
	if (value[*i] == '$')
		(*i)--;
}

void	dollar_expand(char *value, t_ms *minishell, int *i)
{
	char	*word;
	int		start;

	start = *i;
	(*i)++;
	while ((ft_isalnum(value[(*i)]) || value[(*i)] == '_'))
		(*i)++;
	word = ft_substr(value, start, (size_t)((*i) - start));
	fill_expand_lst(minishell, word);
	(*i)--;
}

void	print_expand(t_ms *minishell)
{
	t_token	*tmp;

	tmp = minishell->expand;
	while (tmp)
	{
		printf("node = %s|\n", tmp->value);
		tmp = tmp->next;
	}
}

int	expand_size(t_ms *minishell)
{
	int		i;
	t_token	*tmp;

	i = 0;
	tmp = minishell->expand;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

void	modify_main_token_lst(t_ms *minishell, char *word, int index)
{
	t_token	*tmp;

	tmp = minishell->token;
	while (tmp->index != index)
		tmp = tmp->next;
	free(tmp->value);
	tmp->value = ft_strdup(word);
}
