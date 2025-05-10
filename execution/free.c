/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 02:26:26 by cmontaig          #+#    #+#             */
/*   Updated: 2025/05/10 05:17:24 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_token_list(t_token *token)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token->next;
		free(token->value);
		free(token);
		token = tmp;
	}
}

void	free_cmd_list(t_cmd *cmd)
{
	t_cmd	*tmp;

	while (cmd)
	{
		tmp = cmd->next;
		free_token_list(cmd->token);
		free(cmd->path);
		if (cmd->infile_fd != -2)
			close(cmd->infile_fd);
		if (cmd->outfile_fd != -2)
			close(cmd->outfile_fd);
		free(cmd);
		cmd = tmp;
	}
}

void	free_minishell(t_ms *minishell)
{
	if (!minishell)
		return;
	free_cmd_list(minishell->cmd_list);
	free_token_list(minishell->token);
	free_token_list(minishell->expand);
	free_env(minishell);
	free(minishell->envp);
	free((void *)minishell);
}

void	free_array(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return ;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	free_env(t_ms *minishell)
{
	t_env	*tmp;
	t_env	*next_node;

	tmp = minishell->env_lst;
	while (tmp)
	{
		next_node = tmp->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
		tmp = next_node;
	}
}
