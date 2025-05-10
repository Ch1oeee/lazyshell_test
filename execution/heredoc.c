/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmontaig <cmontaig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 11:15:11 by cmontaig          #+#    #+#             */
/*   Updated: 2025/05/06 14:13:25 by cmontaig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	create_heredoc(char *limiter, t_ms *minishell)
{
	int		fd[2];
	char	*line;
	char	*tmp_line[2];

	if (pipe(fd) == -1)
		return (perror("pipe"), -1);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, limiter) == 0)
		{
			free(line);
			break ;
		}
		tmp_line[0] = line;
		tmp_line[1] = NULL;
		do_expand_heredoc(tmp_line, minishell);
		write(fd[1], tmp_line[0], ft_strlen(tmp_line[0]));
		write(fd[1], "\n", 1);
		free(tmp_line[0]);
	}
	close(fd[1]);
	return (fd[0]);
}

int	setup_heredocs(t_cmd *cmd_list, t_ms *minishell)
{
	t_token	*token;
	int		fd;

	while (cmd_list)
	{
		token = cmd_list->token;
		while (token)
		{
			if (token->type == HEREDOC && token->next && token->next->value)
			{
				fd = create_heredoc(token->next->value, minishell);
				if (fd == -1)
					return (1);
				cmd_list->heredoc_fd = fd;
				cmd_list->is_redir = true;
			}
			token = token->next;
		}
		cmd_list = cmd_list->next;
	}
	return (0);
}
