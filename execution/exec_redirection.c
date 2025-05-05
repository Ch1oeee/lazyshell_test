/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ChloeMontaigut <ChloeMontaigut@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:15:22 by cmontaig          #+#    #+#             */
/*   Updated: 2025/05/05 23:06:24 by ChloeMontai      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int process_redirections(t_ms *minishell)
{
	t_cmd *cmd = minishell->cmd_list;
	t_token *token;

	while (cmd)
	{
		token = cmd->token;
		while (token && token->type != PIPE)
		{
			if ((token->type == REDIR_IN || token->type == REDIR_OUT || token->type == APPEND) &&
				(!token->next || (token->next->type != WORD && token->next->type != D_QUOTE &&
								  token->next->type != S_QUOTE && token->next->type != EXPANDING)))
			{
				ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
				return (1);
			}

			if (token->type == REDIR_IN)
			{
				if (cmd->infile_fd != -2)
					close(cmd->infile_fd);
				cmd->infile_fd = open(token->next->value, O_RDONLY);
				if (cmd->infile_fd == -1)
				{
					ft_putstr_fd("minishell: ", 2);
					perror(token->next->value);
					return (1);
				}
				cmd->is_redir = true;
			}
			else if (token->type == REDIR_OUT)
			{
				if (cmd->outfile_fd != -2)
					close(cmd->outfile_fd);
				cmd->outfile_fd = open(token->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (cmd->outfile_fd == -1)
				{
					ft_putstr_fd("minishell: ", 2);
					perror(token->next->value);
					return (1);
				}
				cmd->is_redir = true;
			}
			else if (token->type == APPEND)
			{
				if (cmd->outfile_fd != -2)
					close(cmd->outfile_fd);
				cmd->outfile_fd = open(token->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
				if (cmd->outfile_fd == -1)
				{
					ft_putstr_fd("minishell: ", 2);
					perror(token->next->value);
					return (1);
				}
				cmd->is_redir = true;
			}

			if (token->type == REDIR_IN || token->type == REDIR_OUT || token->type == APPEND)
				token = token->next;

			token = token->next;
		}
		cmd = cmd->next;
	}
	return (0);
}

void	handle_redirections(t_cmd *cmd, int prev_pipe, int *pipe_fd)
{
	if (cmd->heredoc_fd > 0)
	{
		dup2(cmd->heredoc_fd, STDIN_FILENO);
		close(cmd->heredoc_fd);
	}
	else if (cmd->infile_fd != -2)
	{
		dup2(cmd->infile_fd, STDIN_FILENO);
		close(cmd->infile_fd);
	}
	else if (prev_pipe != -1)
	{
		dup2(prev_pipe, STDIN_FILENO);
		close(prev_pipe);
	}
	if (cmd->outfile_fd != -2)
	{
		dup2(cmd->outfile_fd, STDOUT_FILENO);
		close(cmd->outfile_fd);
	}
	else if (cmd->next)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
	if (cmd->next)
		close(pipe_fd[0]);
}
