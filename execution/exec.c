/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 11:34:45 by skock             #+#    #+#             */
/*   Updated: 2025/04/29 16:16:37 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// void	exec_line(t_ms *minishell)
// {
// 	print_cmd(minishell->cmd_list);
// 	return ;
// }

int	execute_simple_cmd(t_ms *minishell, t_cmd *cmd)
{
	char	**args;
	int		status;

	args = tokens_to_args(cmd->token);
	if (!args || !args[0])
	{
		free_array(args);
		return (0);
	}
	if (is_builtin(args[0]))
	{
		status = execute_builtin(minishell, args);
		free_array(args);
		return (status);
	}
	cmd->path = find_command_path(args[0], minishell->env_lst);
	if (!cmd->path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putstr_fd(args[0], 2);
		write(2, "\n", 1);
		free_array(args);
		return (127);
	}
	cmd->pid = fork();
	if (cmd->pid == 0)
	{
		if (cmd->infile_fd != -2)
			dup2(cmd->infile_fd, STDIN_FILENO);
		if (cmd->outfile_fd != -2)
			dup2(cmd->outfile_fd, STDOUT_FILENO);
		execve(cmd->path, args, minishell->envp);
		ft_putstr_fd("minishell: ", 2);
		perror(args[0]);
		exit(EXIT_FAILURE);
	}
	if (cmd->infile_fd != -2)
		close(cmd->infile_fd);
	if (cmd->outfile_fd != -2)
		close(cmd->outfile_fd);
	waitpid(cmd->pid, &status, 0);
	free_array(args);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	execute_pipeline(t_ms *minishell)
{
	t_cmd	*cmd;
	int		pipe_fd[2];
	int		prev_pipe = -1;
	int		status = 0;
	int		last_pid = -1;

	cmd = minishell->cmd_list;
	while (cmd)
	{
		if (cmd->next && pipe(pipe_fd) == -1)
			return (perror("pipe"), 1);
		if (prev_pipe != -1)
		{
			if (cmd->infile_fd == -2)
				cmd->infile_fd = prev_pipe;
			else
				close(prev_pipe);
		}
		if (cmd->next && cmd->outfile_fd == -2)
			cmd->outfile_fd = pipe_fd[1];
		else if (cmd->next)
			close(pipe_fd[1]);
		char **args = tokens_to_args(cmd->token);
		if (!args || !args[0] || args[0][0] == '\0')
		{
			if (args)
				free_array(args);
			if (prev_pipe != -1)
				close(prev_pipe);
			if (cmd->next)
				close(pipe_fd[0]);
			cmd = cmd->next;
			continue;
		}
		if (!is_builtin(args[0]))
		{
			cmd->path = find_command_path(args[0], minishell->env_lst);
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
		if (is_builtin(args[0]) && !cmd->next && prev_pipe == -1 &&
			cmd->infile_fd == -2 && cmd->outfile_fd == -2)
		{
			status = execute_builtin(minishell, args);
			free_array(args);
		}
		else
		{
			cmd->pid = fork();
			if (cmd->pid == 0)
			{
				if (cmd->infile_fd != -2)
					dup2(cmd->infile_fd, STDIN_FILENO);
				if (cmd->outfile_fd != -2)
					dup2(cmd->outfile_fd, STDOUT_FILENO);
				if (prev_pipe != -1)
					close(prev_pipe);
				if (cmd->next)
				{
					close(pipe_fd[0]);
					if (cmd->outfile_fd != pipe_fd[1])
						close(pipe_fd[1]);
				}
				if (is_builtin(args[0]))
				{
					status = execute_builtin(minishell, args);
					exit(status);
				}
				else if (cmd->path)
				{
					execve(cmd->path, args, minishell->envp);
					ft_putstr_fd("minishell: ", 2);
					perror(args[0]);
					exit(EXIT_FAILURE);
				}
				exit(127);
			}
			if (cmd->infile_fd != -2 && cmd->infile_fd != prev_pipe)
				close(cmd->infile_fd);
			if (cmd->outfile_fd != -2 && (cmd->next == NULL || cmd->outfile_fd != pipe_fd[1]))
				close(cmd->outfile_fd);
			if (prev_pipe != -1)
				close(prev_pipe);
			if (cmd->next)
			{
				close(pipe_fd[1]);
				prev_pipe = pipe_fd[0];
			}
			free_array(args);
			last_pid = cmd->pid;
		}
		cmd = cmd->next;
	}
	cmd = minishell->cmd_list;
	while (cmd)
	{
		if (cmd->pid > 0)
		{
			waitpid(cmd->pid, &status, 0);
			if (cmd->pid == last_pid && WIFEXITED(status))
				status = WEXITSTATUS(status);
		}
		cmd = cmd->next;
	}
	reset_commands(minishell);
	return (status);
}

int exec_line(t_ms *minishell)
{
	int status;
		
	if (process_redirections(minishell) != 0)
		return (1);
		
	if (!minishell->cmd_list->next)
		status = execute_simple_cmd(minishell, minishell->cmd_list);
	else
		status = execute_pipeline(minishell);
		
	minishell->status = status;
	return (status);
}
