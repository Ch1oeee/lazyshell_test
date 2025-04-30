/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_lst.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 19:00:38 by skock             #+#    #+#             */
/*   Updated: 2025/04/10 06:59:13 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmd	*new_cmd(void)
{
	t_cmd	*new;

	new = malloc(sizeof(t_cmd));
	new->path = NULL;
	new->infile_fd = -2;
	new->outfile_fd = -2;
	new->pid = -1;
	new->next = NULL;
	return (new);
}

t_cmd	*create_cmd_until_pipe(t_token *tmp)
{
	t_cmd	*cmd;

	cmd = new_cmd();
	cmd->token = tmp;
	return (cmd);
}

void	cut_weird(t_cmd *cmd)
{
	t_cmd	*tmp_cmd;
	t_token	*tmp_token;

	tmp_cmd = cmd;
	while (tmp_cmd)
	{
		tmp_token = tmp_cmd->token;
		while (tmp_token)
		{
			tmp_token = tmp_token->next;
			if (tmp_token && tmp_token->type == PIPE)
			{
				tmp_token->next = NULL;
				break ;
			}
		}
		tmp_cmd = tmp_cmd->next;
	}
}

void	fill_cmd_lst(t_ms *minishell)
{
	t_token	*tmp;
	t_cmd	*cmd_list;
	t_cmd	*last_cmd;
	t_cmd	*new_cmd_node;

	last_cmd = NULL;
	cmd_list = NULL;
	tmp = minishell->token;
	while (tmp)
	{
		new_cmd_node = create_cmd_until_pipe(tmp);
		if (new_cmd_node)
		{
			if (!cmd_list)
				cmd_list = new_cmd_node;
			else
				last_cmd->next = new_cmd_node;
			last_cmd = new_cmd_node;
		}
		while (tmp && tmp->type != PIPE)
			tmp = tmp->next;
		if (tmp)
			tmp = tmp->next;
	}
	minishell->cmd_list = cmd_list;
}
