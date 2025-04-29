/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:15:22 by cmontaig          #+#    #+#             */
/*   Updated: 2025/04/29 16:16:33 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int process_redirections(t_ms *minishell)
{
	t_cmd *cmd;
	t_token *token;
		
	cmd = minishell->cmd_list;
	while (cmd)
	{
		token = cmd->token;
		while (token && token->type != PIPE)
		{
			if (token->type == REDIR_IN)
			{
				if (cmd->infile_fd != -2)
					close(cmd->infile_fd);
				if (!token->next || (token->next->type != WORD && token->next->type != D_QUOTE 
					&& token->next->type != S_QUOTE && token->next->type != EXPANDING))
					return (ft_putstr_fd("minishell: syntax error near unexpected token\n", 2), 1);
				token = token->next;
				cmd->infile_fd = open(token->value, O_RDONLY);
				if (cmd->infile_fd == -1)
				{
					ft_putstr_fd("minishell: ", 2);
					perror(token->value);
					return (1);
				}
				cmd->is_redir = true;
			}
			else if (token->type == REDIR_OUT)
			{
				if (cmd->outfile_fd != -2)
					close(cmd->outfile_fd);
				if (!token->next || (token->next->type != WORD && token->next->type != D_QUOTE 
					&& token->next->type != S_QUOTE && token->next->type != EXPANDING))
					return (ft_putstr_fd("minishell: syntax error near unexpected token\n", 2), 1);
				token = token->next;
				cmd->outfile_fd = open(token->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (cmd->outfile_fd == -1)
				{
					ft_putstr_fd("minishell: ", 2);
					perror(token->value);
					return (1);
				}
				cmd->is_redir = true;
			}
			else if (token->type == APPEND)
			{
				if (cmd->outfile_fd != -2)
					close(cmd->outfile_fd);
				if (!token->next || (token->next->type != WORD && token->next->type != D_QUOTE 
					&& token->next->type != S_QUOTE && token->next->type != EXPANDING))
					return (ft_putstr_fd("minishell: syntax error near unexpected token\n", 2), 1);
				token = token->next;
				cmd->outfile_fd = open(token->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
				if (cmd->outfile_fd == -1)
				{
					ft_putstr_fd("minishell: ", 2);
					perror(token->value);
					return (1);
				}
				cmd->is_redir = true;
			}
			if (!token) break;
			token = token->next;
		}
		if (cmd->next)
			cmd->is_pipe = true;
		cmd = cmd->next;
	}
	return (0);
}
