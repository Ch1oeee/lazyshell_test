/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:14:03 by cmontaig          #+#    #+#             */
/*   Updated: 2025/05/10 05:16:46 by cmontaig         ###   ########.fr       */
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
void print_cmd_not_found(char *cmd)
{
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found", 2);
	write(2, "\n", 1);
}

void cleanup_pipes(t_cmd *cmd, int pipe_fd[2], int *prev_pipe)
{
	if (*prev_pipe != -1)
		close(*prev_pipe);
	if (cmd->next && pipe_fd[0] != -1)
		close(pipe_fd[0]);
}

int	setup_pipes(t_cmd *cmd, int *pipe_fd, int *prev_pipe)
{
	if (cmd->next && pipe(pipe_fd) == -1)
		return (perror("pipe"), 1);
	if (*prev_pipe != -1)
	{
		if (cmd->infile_fd == -2)
			cmd->infile_fd = *prev_pipe;
		else
			close(*prev_pipe);
	}
	if (cmd->next && cmd->outfile_fd == -2)
		cmd->outfile_fd = pipe_fd[1];
	else if (cmd->next)
		close(pipe_fd[1]);
	return (0);
}

// void	update_fds(t_cmd *cmd, int *pipe_fd, int *prev_pipe)
// {
// 	if (cmd->infile_fd != -2 && cmd->infile_fd != *prev_pipe)
// 		close(cmd->infile_fd);
// 	if (cmd->outfile_fd != -2 && (!cmd->next || cmd->outfile_fd != pipe_fd[1]))
// 		close(cmd->outfile_fd);
// 	if (*prev_pipe != -1)
// 		close(*prev_pipe);
// 	if (cmd->heredoc_fd > 0)
// 		close(cmd->heredoc_fd);
// 	if (cmd->next)
// 	{
// 		close(pipe_fd[1]);
// 		*prev_pipe = pipe_fd[0];
// 	}
// }

void update_fds(t_cmd *cmd, int *pipe_fd, int *prev_pipe)
{
    // Vérifiez que les descripteurs sont valides avant de les manipuler
    if (cmd->infile_fd != -2 && cmd->infile_fd != *prev_pipe)
        close(cmd->infile_fd);
        
    if (cmd->outfile_fd != -2 && (!cmd->next || cmd->outfile_fd != pipe_fd[1]))
        close(cmd->outfile_fd);
        
    if (*prev_pipe != -1)
        close(*prev_pipe);
        
    if (cmd->heredoc_fd > 0)
        close(cmd->heredoc_fd);
        
    if (cmd->next && pipe_fd[1] != -1)  // Vérification ajoutée
    {
        close(pipe_fd[1]);
        *prev_pipe = pipe_fd[0];
    }
    else
    {
        *prev_pipe = -1;  // Assurez-vous de réinitialiser si aucun pipe n'est créé
    }
}