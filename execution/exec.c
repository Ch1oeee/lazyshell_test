/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:34:45 by skock             #+#    #+#             */
/*   Updated: 2025/05/10 05:52:35 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_pipeline(t_ms *minishell)
{
	t_cmd	*cmd = minishell->cmd_list;
	int		pipe_fd[2];
	int		prev_pipe = -1;
	int		last_pid = -1;
	int		redir_ok;

	while (cmd)
	{
		redir_ok = 1;
		if (process_redirections(cmd, minishell))
		{
			minishell->status = 1;
			redir_ok = 0;
		}
		if (setup_pipes(cmd, pipe_fd, &prev_pipe))
			return (1);
		if (redir_ok)
			last_pid = handle_command(minishell, cmd, pipe_fd, &prev_pipe, &minishell->status);
		else
			cleanup_pipes(cmd, pipe_fd, &prev_pipe); 
		cmd = cmd->next;
	}
	return (wait_all_children(minishell, last_pid, minishell->status));
}

int	handle_command(t_ms *ms, t_cmd *cmd, int pipe_fd[2], int *prev_pipe, int *status)
{
	char **args = tokens_to_args(cmd->token);
	int	pid;

	if (!args || !args[0])
	{
		pid = handle_empty_cmd(cmd, prev_pipe, pipe_fd, ms);
		free_array(args);
		return (pid);
	}
	if (!is_builtin(args[0]))
	{
		cmd->path = find_command_path(args[0], ms->env_lst);
		if (!cmd->path)
		{
			print_cmd_not_found(args[0]);
			free_array(args);
			cleanup_pipes(cmd, pipe_fd, prev_pipe);
			return (ms->status = 127, -1);
		}
	}
	pid = execute_cmd(ms, cmd, args, pipe_fd, *prev_pipe, status);
	update_fds(cmd, pipe_fd, prev_pipe);
	free_array(args);
	return (pid);
}

int	handle_empty_cmd(t_cmd *cmd, int *prev_pipe, int pipe_fd[2], t_ms *ms)
{	
	if (!cmd->is_redir)
	{
		ft_putstr_fd(": command not found", 2);
		cleanup_pipes(cmd, pipe_fd, prev_pipe);
		return (ms->status = 127, -1);
	}
	int child_pid = fork();
	if (child_pid == 0)
	{
		handle_redirections(cmd, *prev_pipe, pipe_fd);
		exit(EXIT_SUCCESS);
	}
	else if (child_pid > 0)
		cmd->pid = child_pid;
	cleanup_pipes(cmd, pipe_fd, prev_pipe);
	return (child_pid);
}

int	execute_cmd(t_ms *minishell, t_cmd *cmd, char **args, int *pipe_fd, int prev, int *status)
{
	if (!args || !args[0])
		return (-1);
	if (is_builtin(args[0]) && !cmd->next && prev == -1 &&
		cmd->infile_fd == -2 && cmd->outfile_fd == -2)
	{
		*status = execute_builtin(minishell, args);
		return (-1);
	}
	cmd->pid = fork();
	if (cmd->pid == 0)
	{
		if (cmd->heredoc_fd > 0)
			dup2(cmd->heredoc_fd, STDIN_FILENO);
		else if (cmd->infile_fd != -2)
			dup2(cmd->infile_fd, STDIN_FILENO);
		else if (prev != -1)
			dup2(prev, STDIN_FILENO);
		if (cmd->outfile_fd != -2)
			dup2(cmd->outfile_fd, STDOUT_FILENO);
		else if (cmd->next)
			dup2(pipe_fd[1], STDOUT_FILENO);
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
		if (!cmd->path)
			exit(EXIT_FAILURE);
		execve(cmd->path, args, minishell->envp);
		if (errno == EACCES)
		{
			ft_putstr_fd("minishell: permission denied: ", 2);
			ft_putstr_fd(args[0], 2);
			ft_putchar_fd('\n', 2);
			minishell->status = 126;
			exit(minishell->status);
		}
		else
		{
			ft_putstr_fd("minishell: ", 2);
			perror(args[0]);
			exit(EXIT_FAILURE);
		}
	}
	else if (cmd->pid < 0)
	{
		perror("minishell: fork");
		return (-1);
	}
	return (cmd->pid);
}

int	wait_all_children(t_ms *ms, int last_pid, int last_status)
{
	t_cmd	*cmd = ms->cmd_list;
	int		status;

	while (cmd)
	{
		if (cmd->pid > 0)
		{
			waitpid(cmd->pid, &status, 0);
			if (cmd->pid == last_pid && WIFEXITED(status))
			{
				last_status = WEXITSTATUS(status);
				ms->status = last_status;
			}
		}
		cmd = cmd->next;
	}
	return (last_status);
}
