/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:43:47 by skock             #+#    #+#             */
/*   Updated: 2025/05/10 05:17:55 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error_message(const char *msg, t_ms *minishell)
{
	char	*special;

	if (minishell->first_special != 69 && minishell->second_special != 69)
	{
		if (minishell->second_special == HEREDOC)
			special = ft_strdup("<<");
		if (minishell->second_special == APPEND)
			special = ft_strdup(">>");
		if (minishell->second_special == REDIR_IN)
			special = ft_strdup("<");
		if (minishell->second_special == REDIR_OUT)
			special = ft_strdup(">");
		if (minishell->second_special == PIPE)
			special = ft_strdup("|");
		ft_printf("syntax error near unexpected token '%s'\n", special);
	}
	else
		ft_printf("%s\n", msg);
	// minishell->go_cmd == false;
}

void	prompt(t_ms *minishell)
{
	char	*input;

	while (1)
	{
		char	*cwd;
		char	*full_prompt;
		char	*last;
		
		cwd = getcwd(NULL, 0);
		last = get_last_dir(cwd);
		full_prompt = ft_strjoin(last, " > ");
		free(cwd);
		input = readline(full_prompt);
		free(full_prompt);
		if (!input)
		{
			ft_printf("CTRL + D\n");
			free_env(minishell);
			free(minishell);
			exit(0);
		}
		if (!parsing_input(input, minishell))
 		{
 			print_error_message("error", minishell);
 			continue;
 		}
		if (input && *input)
			add_history(input);
		if (setup_heredocs(minishell->cmd_list, minishell) < 0)
		{
			print_error_message("heredoc failed", minishell);
			free(input);
			free_env(minishell);
			free(minishell);
			exit(1);
		}
		if (minishell->cmd_list)
		{
			execute_pipeline(minishell);
			free_cmd_list(minishell->cmd_list);
			minishell->cmd_list = NULL;
		}
		free(input);
	}
}

//add-on chloe

int	create_token_chain(t_token *first_token, char **args)
{
	t_token	*current;
	t_token	*new_token;
	int		i;

	current = first_token;
	i = 1;
	while (args[i])
	{
		new_token = malloc(sizeof(t_token));
		if (!new_token)
			return (1);
		new_token->value = args[i];
		new_token->is_next_space = true;
		new_token->type = WORD;
		new_token->index = i;
		new_token->next = NULL;
		current->next = new_token;
		current = new_token;
		i++;
	}
	return (0);
}

int	run_builtin_command(t_ms *minishell, t_cmd *cmd, char **args)
{
	int	result = 0;
	
	if (!ft_strcmp(args[0], "echo"))
		print_echo(cmd);
	else if (!ft_strcmp(args[0], "cd"))
		cd(cmd, minishell);
	else if (!ft_strcmp(args[0], "pwd"))
		print_pwd();
	else if (!ft_strcmp(args[0], "export"))
		result = ft_export(minishell, cmd);
	else if (!ft_strcmp(args[0], "unset"))
		ft_unset(minishell, cmd);
	else if (!ft_strcmp(args[0], "env"))
		print_env(minishell);
	else if (!ft_strcmp(args[0], "exit"))
		ft_exit(cmd, minishell);
	else
		return (minishell->status = 1, 1);
	minishell->status = result;
	return (result);
}

int	execute_builtin(t_ms *minishell, char **args)
{
	t_cmd	cmd;
	t_token	first_token;
	t_token	*current;
	int		result;

	if (!args || !args[0])
		return (1);
	cmd.path = NULL;
	cmd.infile_fd = -2;
	cmd.outfile_fd = -2;
	cmd.is_pipe = false;
	cmd.is_redir = false;
	cmd.pid = -1;
	cmd.next = NULL;

	first_token.value = args[0];
	first_token.is_next_space = true;
	first_token.type = WORD;
	first_token.index = 0;
	first_token.next = NULL;
	
	if (create_token_chain(&first_token, args))
		return (1);
		
	cmd.token = &first_token;
	result = run_builtin_command(minishell, &cmd, args);
	
	current = first_token.next;
	while (current)
	{
		t_token *to_free = current;
		current = current->next;
		free(to_free);
	}
	return (result);
}

char	*get_last_dir(char *path)
{
	char	*last_slash = ft_strrchr(path, '/');
	if (!last_slash)
		return (path);
	return (last_slash + 1);
}

int	main(int ac, char **av, char **envp)
{
	t_ms	*minishell;

	(void)av;
	if (ac == 1)
	{
		minishell = malloc(sizeof(t_ms));
		if (!minishell)
			return (1);
		minishell->envp = envp;
		minishell->is_next_space = false;
		minishell->first_special = 69;
		minishell->second_special = 69;
		minishell->status = 0;
		minishell->go_cmd = true;
		minishell->token = NULL;
		minishell->expand = NULL;
		minishell->cmd_list = NULL;
		minishell->pipe_fd[0] = -1;
		minishell->pipe_fd[1] = -1;
		fill_env_cpy(minishell, envp);
		prompt(minishell);
		// exec_line(minishell);
		free_minishell(minishell);
		return (0);
	}
	return (1);
}
