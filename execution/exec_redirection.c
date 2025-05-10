/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:15:22 by cmontaig          #+#    #+#             */
/*   Updated: 2025/05/10 05:05:57 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	process_redirections(t_cmd *cmd, t_ms *ms)
{
	t_token	*token = cmd->token;
	int		fd;

	while (token && token->type != PIPE)
	{
		if ((token->type == REDIR_IN || token->type == REDIR_OUT || token->type == APPEND) &&
			(!token->next || (token->next->type != WORD && token->next->type != D_QUOTE &&
							  token->next->type != S_QUOTE && token->next->type != EXPANDING)))
		{
			ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
			ms->status = 1;
			return (1);
		}
		if (token->type == REDIR_IN)
		{
			if (cmd->infile_fd != -2)
				close(cmd->infile_fd);
			fd = open(token->next->value, O_RDONLY);
			if (fd == -1)
			{
				ft_putstr_fd("minishell: ", 2);
				perror(token->next->value);
				ms->status = 1;
				if (cmd->infile_fd != -2 && cmd->infile_fd != -1)
					close(cmd->infile_fd);
				return (1); 
			}
			cmd->infile_fd = fd;
			cmd->is_redir = true;
		}
		else if (token->type == REDIR_OUT)
		{
			if (cmd->outfile_fd != -2)
				close(cmd->outfile_fd);
			fd = open(token->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				ft_putstr_fd("minishell: ", 2);
				perror(token->next->value);
				ms->status = 1;
				if (cmd->infile_fd != -2 && cmd->infile_fd != -1)
					close(cmd->infile_fd);
				return (1);
			}
			cmd->outfile_fd = fd;
			cmd->is_redir = true;
		}
		else if (token->type == APPEND)
		{
			if (cmd->outfile_fd != -2)
				close(cmd->outfile_fd);
			fd = open(token->next->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
			{
				ft_putstr_fd("minishell: ", 2);
				perror(token->next->value);
				ms->status = 1;
				if (cmd->infile_fd != -2 && cmd->infile_fd != -1)
					close(cmd->infile_fd);
				return (1);
			}
			cmd->outfile_fd = fd;
			cmd->is_redir = true;
		}
		if (token->type == REDIR_IN || token->type == REDIR_OUT || token->type == APPEND)
			token = token->next;
		token = token->next;
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
