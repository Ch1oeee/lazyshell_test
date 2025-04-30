/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:34:45 by skock             #+#    #+#             */
/*   Updated: 2025/04/30 14:27:59 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_pipeline(t_ms *minishell)
{
	t_cmd	*cmd = minishell->cmd_list;
	int		pipe_fd[2];
	int		prev_pipe = -1;
	int		status = 0;
	int		last_pid = -1;
	char	**args;

	while (cmd)
	{
		if (setup_pipes(cmd, pipe_fd, &prev_pipe))
			return (1);
		args = tokens_to_args(cmd->token);
		if (!is_builtin(args[0]))
		{
			cmd->path = find_command_path(args[0], minishell->env_lst);
			print_cmd(cmd);
			if (!cmd->path)
			{
				ft_putstr_fd("minishell: command not found: ", 2);
				ft_putstr_fd(args[0], 2);
				write(2, "\n", 1);
				free_array(args);
				if (prev_pipe != -1)
					close(prev_pipe);
				if (cmd->next)
					close(pipe_fd[0]);
				return (127);
			}
		}
		last_pid = execute_cmd(minishell, cmd, args, pipe_fd, prev_pipe, &status);
		update_fds(cmd, pipe_fd, &prev_pipe);
		free_array(args);
		cmd = cmd->next;
	}
	return (wait_all_children(minishell, last_pid, status));
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

int	execute_cmd(t_ms *minishell, t_cmd *cmd, char **args, int *pipe_fd, int prev, int *status)
{
	if (is_builtin(args[0]) && !cmd->next && prev == -1 &&
		cmd->infile_fd == -2 && cmd->outfile_fd == -2)
	{
		*status = execute_builtin(minishell, args);
		return (-1);
	}
	cmd->pid = fork();
	if (cmd->pid == 0)
	{
		if (cmd->infile_fd != -2)
			dup2(cmd->infile_fd, STDIN_FILENO);
		if (cmd->outfile_fd != -2)
			dup2(cmd->outfile_fd, STDOUT_FILENO);
		if (prev != -1)
			close(prev);
		if (cmd->next)
		{
			close(pipe_fd[0]);
			if (cmd->outfile_fd != pipe_fd[1])
				close(pipe_fd[1]);
		}
		if (is_builtin(args[0]))
			exit(execute_builtin(minishell, args));
		execve(cmd->path, args, minishell->envp);
		ft_putstr_fd("minishell: ", 2);
		perror(args[0]);
		exit(EXIT_FAILURE);
	}
	return (cmd->pid);
}

void	update_fds(t_cmd *cmd, int *pipe_fd, int *prev_pipe)
{
	if (cmd->infile_fd != -2 && cmd->infile_fd != *prev_pipe)
		close(cmd->infile_fd);
	if (cmd->outfile_fd != -2 && (!cmd->next || cmd->outfile_fd != pipe_fd[1]))
		close(cmd->outfile_fd);
	if (*prev_pipe != -1)
		close(*prev_pipe);
	if (cmd->next)
	{
		close(pipe_fd[1]);
		*prev_pipe = pipe_fd[0];
	}
}

int	wait_all_children(t_ms *minishell, int last_pid, int last_status)
{
	t_cmd	*cmd;
	int		status;

	cmd = minishell->cmd_list;
	while (cmd)
	{
		if (cmd->pid > 0)
		{
			waitpid(cmd->pid, &status, 0);
			if (cmd->pid == last_pid && WIFEXITED(status))
				last_status = WEXITSTATUS(status);
		}
		cmd = cmd->next;
	}
	reset_commands(minishell);
	return (last_status);
}

