/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 16:43:03 by cmontaig          #+#    #+#             */
/*   Updated: 2025/04/29 14:00:31 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_echo(t_cmd *cmd)
{
	t_token	*token;
	int		newline;
	bool	first;
	
	newline = 1;
	first = true;
	token = cmd->token->next;
	if (token && token->type == WORD && ft_strcmp(token->value, "-n") == 0)
	{
		newline = 0;
		token = token->next;
	}
	while (token && token->type == WORD)
	{
		if (!first)
			write(STDOUT_FILENO, " ", 1);
		
		write(STDOUT_FILENO, token->value, ft_strlen(token->value));
		first = false;
		token = token->next;
	}
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
}
