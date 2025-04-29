/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:14:03 by cmontaig          #+#    #+#             */
/*   Updated: 2025/04/29 16:14:50 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool is_builtin(char *cmd)
{
	if (!cmd)
		return (false);
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") || 
			!ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export") || 
			!ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env") || 
			!ft_strcmp(cmd, "exit"));
}

void reset_commands(t_ms *minishell)
{
 	t_cmd *cmd;
 	t_cmd *next;
 	
 	if (!minishell)
 		return;
 	if (!minishell->cmd_list)
 		return;
 	cmd = minishell->cmd_list;
 	while (cmd)
 	{
 		next = cmd->next;
 		if (cmd->path)
 			free(cmd->path);
 		if (cmd->infile_fd > 2)
 			close(cmd->infile_fd);
 		if (cmd->outfile_fd > 2)
 			close(cmd->outfile_fd);
 		free(cmd);
 		cmd = next;
 	}
 	minishell->cmd_list = NULL;
}
