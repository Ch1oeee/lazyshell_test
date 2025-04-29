/* ************************************************************************** */
/*	                                                                        */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 17:54:49 by cmontaig          #+#    #+#             */
/*   Updated: 2025/04/12 13:13:17 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_exit(t_cmd *cmd, t_ms *minishell)
{
	int		exit_status;
	t_token	*token;
	
	exit_status = 0;
	printf("exit\n");

	if (cmd->token->next)
	{
		token = cmd->token->next;
		exit_status = ft_atoi(token->value);
		if (!ft_isdigit(*token->value))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_printf("%s :", token->value);
			ft_putstr_fd("numeric argument required\n", 2);
			return;
		}
		if (token->next) {
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			return;
		}
	}
	free_env(minishell);
	free(minishell);
	exit(exit_status);
}


