/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 12:58:30 by skock             #+#    #+#             */
/*   Updated: 2025/05/06 15:21:23 by cmontaig         ###   ########.fr       */
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
			ft_printf("%s", tmp->key);
			ft_printf("=");
			ft_printf("%s\n", tmp->value);
			tmp = tmp->next;
		}
		else
			tmp = tmp->next;
	}
	return ;
}
