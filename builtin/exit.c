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

// +100 works but ++100 doesn't
void	ft_exit(t_cmd *cmd, t_ms *minishell)
{
	int		exit_status;
	t_token	*token;
	
	exit_status = 0;
	if (cmd->token->next)
	{
		token = cmd->token->next;
		exit_status = ft_atoi(token->value);
		if (!double_sign(token->value))
		{
			ft_putstr_fd("exit\n", 2);
			ft_putstr_fd("minishell: exit: ", 2);
			ft_printf("%s: ", token->value);
			ft_putstr_fd("numeric argument required\n", 2);
			exit_status = 2;
		}
		if (token->next) 
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			exit_status = 1;
		}
	}
	free_env(minishell);
	free(minishell);
	exit(exit_status);
}

int	double_sign(char *str)
{
	int	i;

	i = 0;
	if(str[i] == '+' || str[i] == '-')
		i++;
	if (!ft_isdigit(str[i]))
		return (0);
	while(str[i])
	{
		if (!ft_isdigit(str[i]))
		return (0);
		i++;
	}
	return (1);
}
