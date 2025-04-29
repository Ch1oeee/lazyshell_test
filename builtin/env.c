/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 12:58:30 by skock             #+#    #+#             */
/*   Updated: 2025/03/12 12:58:32 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_env(t_ms *minishell)
{
	t_env	*tmp;

	tmp = minishell->env_lst;
	while (tmp)
	{
		if (tmp->value)
		{
			printf("%s", tmp->key);
			printf("=");
			printf("%s\n", tmp->value);
			tmp = tmp->next;
		}
		else
			tmp = tmp->next;
	}
	return ;
}
