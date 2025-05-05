#include "../minishell.h"

int	create_heredoc(char *limiter)
{
	int		fd[2];
	char	*line;

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
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	close(fd[1]);
	return (fd[0]);
}

int	setup_heredocs(t_cmd *cmd_list)
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
				fd = create_heredoc(token->next->value);
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

