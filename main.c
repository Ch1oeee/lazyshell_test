/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 10:43:47 by skock             #+#    #+#             */
/*   Updated: 2025/04/29 16:00:06 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error_message(const char *msg)
{
	printf("%s\n", msg);
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
			printf("CTRL + D\n");
			free_env(minishell);
			free(minishell);
			exit(0);
		}
		if (!parsing_input(input, minishell))
			print_error_message("error");
		if (input && *input)
			add_history(input);
		if (minishell->cmd_list)
			exec_line(minishell);
		free(input);
	}
}

//add-on chloe

// int	execute_builtin(t_cmd *cmd, t_ms *minishell)
// {
// 	char *cmd_name;

// 	if (!cmd || !cmd->token)
// 		return (0);
// 	cmd_name = cmd->token->value;
// 	if (!ft_strcmp(cmd_name, "echo"))
// 		print_echo(cmd);
// 	else if (!ft_strcmp(cmd_name, "pwd"))
// 		print_pwd();
// 	else if (!ft_strcmp(cmd_name, "env"))
// 		print_env(minishell);
// 	else if (!ft_strcmp(cmd_name, "cd"))
// 		cd(cmd, minishell);
// 	else if (!ft_strcmp(cmd_name, "exit"))
// 		ft_exit(cmd, minishell);
// 	else if (!ft_strcmp(cmd_name, "export"))
// 		ft_export(minishell, cmd);
// 	else if (!ft_strcmp(cmd_name, "unset"))
// 		ft_unset(minishell, cmd);
// 	else
// 		return (0);
// 	return (1);
// }


int	execute_builtin(t_ms *minishell, char **args)
{
	t_cmd	temp_cmd;
	t_token	first_token;
	t_token	*current;
	int	result;
	
	current = &first_token;
	if (!args || !args[0])
		return (1);
	temp_cmd.path = NULL;
	temp_cmd.infile_fd = -2;
	temp_cmd.outfile_fd = -2;
	temp_cmd.is_pipe = false;
	temp_cmd.is_redir = false;
	temp_cmd.pid = -1;
	temp_cmd.next = NULL;
		
	first_token.value = args[0];
	first_token.is_next_space = true;
	first_token.type = WORD;
	first_token.index = 0;
	first_token.next = NULL;
		
	for (int i = 1; args[i]; i++)
	{
		t_token *new_token = malloc(sizeof(t_token));
		if (!new_token)
	   return (1);
	   
		new_token->value = args[i];
		new_token->is_next_space = true;
		new_token->type = WORD;
		new_token->index = i;
		new_token->next = NULL;
		
		current->next = new_token;
		current = new_token;
	}
	temp_cmd.token = &first_token;
	result = 0;
	if (!ft_strcmp(args[0], "echo"))
		print_echo(&temp_cmd);
	else if (!ft_strcmp(args[0], "cd"))
		cd(&temp_cmd, minishell);
	else if (!ft_strcmp(args[0], "pwd"))
		print_pwd();
	else if (!ft_strcmp(args[0], "export"))
		ft_export(minishell, &temp_cmd);
	else if (!ft_strcmp(args[0], "unset"))
		ft_unset(minishell, &temp_cmd);
	else if (!ft_strcmp(args[0], "env"))
		print_env(minishell);
	else if (!ft_strcmp(args[0], "exit"))
		ft_exit(&temp_cmd, minishell);
	else
		result = 1;
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
		minishell->envp = envp;
		minishell->is_next_space = false;
		fill_env_cpy(minishell, envp);
		prompt(minishell);
		// exec_line(minishell);
		return (0);
	}
	return (1);
}


